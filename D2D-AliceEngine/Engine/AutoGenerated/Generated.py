# -*- coding: utf-8 -*-
import os
import re

type_cast_map = {
    "int": "int",
    "float": "float",
    "double": "double",
    "char": "char",
    "bool": "bool",
    "short": "short",
    "long": "long",
    "long long": "long long",
    "unsigned char": "unsigned char",
    "unsigned int": "unsigned int",
    "unsigned short": "unsigned short",
    "unsigned long": "unsigned long",
    "unsigned long long": "unsigned long long",
    "wchar_t": "wchar_t",
    "long double": "long double",
    "std::string": "std::string",
    "std::wstring": "std::wstring",
    "FAABB": "FAABB",
    "Transform": "Transform",
    "FColor": "FColor",
    "FVector2": "FVector2",
    # Add more project-specific struct/enum/pointer types as needed
}

def try_read(path):
    for enc in ['utf-8', 'cp949', 'euc-kr', 'latin1']:
        try:
            with open(path, encoding=enc) as f:
                return f.read()
        except UnicodeDecodeError:
            continue
        except Exception as e:
            print(f"Warning: Could not read {path}: {e}")
            return None
    print(f"Warning: Cannot decode {path} with tried encodings.")
    return None

def find_files(root, pattern):
    for dirpath, _, filenames in os.walk(root):
        for fname in filenames:
            if fname.endswith(pattern):
                yield os.path.join(dirpath, fname)

def extract_enum_classes(header_code):
    enums = []
    enum_pattern = re.compile(r'enum\s+class\s+(\w+)')
    for match in enum_pattern.finditer(header_code):
        enums.append(match.group(1))
    return enums

def extract_structs(header_code):
    # Only match struct definitions with body (not forward declarations)
    structs = []
    struct_pattern = re.compile(r'struct\s+(\w+)\s*{')
    for match in struct_pattern.finditer(header_code):
        structs.append(match.group(1))
    return structs

def extract_classes(header_code):
    # Only match class definitions with body (not forward declarations)
    # e.g. class Foo { ... };
    class_pattern = re.compile(r'class\s+(\w+)(\s*:\s*public\s+(\w+))?\s*{([\s\S]*?)};', re.MULTILINE)
    member_pattern = re.compile(r'([\w:<>]+)\s+(\w+);')
    classes = []
    for class_match in class_pattern.finditer(header_code):
        class_name = class_match.group(1)
        parent = class_match.group(3) or ""
        body = class_match.group(4)
        members = []
        for m in member_pattern.finditer(body):
            t, n = m.group(1).strip(), m.group(2).strip()
            members.append((t, n))
        classes.append((class_name, parent, members))
    return classes

def scan_all_headers(project_roots):
    all_classes = []
    seen_files = set()
    seen_definitions = set()  # Only definitions with body!
    for root_dir in project_roots:
        for path in find_files(root_dir, ".h"):
            if path in seen_files:
                continue
            seen_files.add(path)
            code = try_read(path)
            if code is None:
                continue
            for c in extract_classes(code):
                # Only register if this is the first definition with body
                if c[0] not in seen_definitions:
                    all_classes.append(c)
                    seen_definitions.add(c[0])
    return all_classes

def generate_register_member_code(classes):
    code = []
    seen = set()
    for cname, parent, members in classes:
        if cname in seen:
            continue
        seen.add(cname)
        code.append(f'// {cname} member registration')
        code.append(f'std::string className = typeid({cname}).name();')
        for t, n in members:
            code.append(f'REGISTER_MEMBER(allMemberInfos[className], {cname}, {n});')
        code.append('')
    return '\n'.join(code)

def topo_sort_classes(classes):
    graph = {}
    indegree = {}
    class_map = {c[0]: c for c in classes}
    for cname, parent, _ in classes:
        graph.setdefault(cname, set())
        indegree.setdefault(cname, 0)
        if parent:
            graph.setdefault(parent, set()).add(cname)
            indegree[cname] = indegree.get(cname, 0) + 1
            indegree.setdefault(parent, 0)
    sorted_classes = []
    queue = [c for c in indegree if indegree[c] == 0]
    seen = set()
    while queue:
        node = queue.pop(0)
        if node in class_map and node not in seen:
            sorted_classes.append(class_map[node])
            seen.add(node)
        for child in graph.get(node, []):
            indegree[child] -= 1
            if indegree[child] == 0:
                queue.append(child)
    for c in classes:
        if c[0] not in [x[0] for x in sorted_classes]:
            sorted_classes.append(c)
    return sorted_classes

def generate_meta_info_code(classes):
    sorted_classes = topo_sort_classes(classes)
    code = []
    seen = set()
    for cname, parent, members in sorted_classes:
        if cname in seen:
            continue
        seen.add(cname)
        # If no members, use WithoutCreator (but only for real definitions)
        if len(members) == 0:
            code.append(f'RegisterMetaInfoTypeWithoutCreator<{cname}>();')
        else:
            if parent:
                code.append(f'RegisterMetaInfoType<{cname}>({{ "{parent}" }});')
            else:
                code.append(f'RegisterMetaInfoType<{cname}>();')
    return '\n'.join(code)

def generate_type_cast_code(classes, enums, structs):
    code = []
    used_types = set(type_cast_map.values())
    for cname, parent, members in classes:
        for mtype, _ in members:
            used_types.add(mtype)
    for enum in enums:
        used_types.add(f"Define::{enum}")
    for struct in structs:
        used_types.add(struct)
    code.append('/* Auto-generated: type-casting for replication */')
    for t in sorted(used_types):
        code.append(f'''else if (type == typeid({t}).name()) {{
    {t}* dstVal = reinterpret_cast<{t}*>(reinterpret_cast<char*>(dst) + dstOffset);
    const {t}* srcVal = reinterpret_cast<const {t}*>(reinterpret_cast<const char*>(src) + srcOffset);
    *dstVal = *srcVal;
}}''')
    return '\n'.join(code)

def main():
    # Example: multiple header root directories
    project_roots = ["../Component", "../Object"]  # Add more as needed
    define_path = "../Define/Define.h"
    structs_path = "../Animation/TextureLoader.h"

    # 1. Extract enum class types
    define_code = try_read(define_path)
    enums = extract_enum_classes(define_code) if define_code else []

    # 2. Extract struct types (only struct with body, not forward declaration)
    structs_code = try_read(structs_path)
    structs = extract_structs(structs_code) if structs_code else []

    # 3. Extract all classes and members from multiple roots (only classes with body)
    all_classes = scan_all_headers(project_roots)

    # 4. REGISTER_MEMBER auto-generation
    with open("AutoRegisterMember.inc", "w", encoding="utf-8") as f:
        f.write("// --- Auto-generated REGISTER_MEMBER code ---\n")
        f.write(generate_register_member_code(all_classes))

    # 5. RegisterMetaInfoType auto-generation (parent before child, no duplicates)
    with open("AutoMetaInfo.inc", "w", encoding="utf-8") as f:
        f.write("// --- Auto-generated RegisterMetaInfoType code ---\n")
        f.write(generate_meta_info_code(all_classes))

    # 6. Type-casting replication code auto-generation
    with open("AutoTypePattern.inc", "w", encoding="utf-8") as f:
        f.write("// --- Auto-generated type-casting code ---\n")
        f.write(generate_type_cast_code(all_classes, enums, structs))

    print("Auto code generation done: AutoRegisterMember.inc, AutoMetaInfo.inc, AutoTypePattern.inc")

if __name__ == "__main__":
    main()
