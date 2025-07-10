// --- Auto-generated REGISTER_MEMBER code ---
// Animator member registration
std::string className = typeid(Animator).name();
REGISTER_MEMBER(allMemberInfos[className], Animator, bLoopping);
REGISTER_MEMBER(allMemberInfos[className], Animator, curAnimationClip);
REGISTER_MEMBER(allMemberInfos[className], Animator, nextAnimationClip);
REGISTER_MEMBER(allMemberInfos[className], Animator, sheet);
REGISTER_MEMBER(allMemberInfos[className], Animator, animationClips);
REGISTER_MEMBER(allMemberInfos[className], Animator, m_bitmap);

// BoxComponent member registration
std::string className = typeid(BoxComponent).name();
REGISTER_MEMBER(allMemberInfos[className], BoxComponent, m_color);
REGISTER_MEMBER(allMemberInfos[className], BoxComponent, thickness);
REGISTER_MEMBER(allMemberInfos[className], BoxComponent, eboxtype);
REGISTER_MEMBER(allMemberInfos[className], BoxComponent, m_pBrush);
REGISTER_MEMBER(allMemberInfos[className], BoxComponent, m_color);
REGISTER_MEMBER(allMemberInfos[className], BoxComponent, m_size);

// Collider member registration
std::string className = typeid(Collider).name();
REGISTER_MEMBER(allMemberInfos[className], Collider, aabb);

// InputComponent member registration
std::string className = typeid(InputComponent).name();
REGISTER_MEMBER(allMemberInfos[className], InputComponent, handle);
REGISTER_MEMBER(allMemberInfos[className], InputComponent, func);

// RenderComponent member registration
std::string className = typeid(RenderComponent).name();

// ScriptComponent member registration
std::string className = typeid(ScriptComponent).name();

// SpriteRenderer member registration
std::string className = typeid(SpriteRenderer).name();
REGISTER_MEMBER(allMemberInfos[className], SpriteRenderer, filePath);
REGISTER_MEMBER(allMemberInfos[className], SpriteRenderer, m_bitmap);

// StatComponent member registration
std::string className = typeid(StatComponent).name();
REGISTER_MEMBER(allMemberInfos[className], StatComponent, value);
REGISTER_MEMBER(allMemberInfos[className], StatComponent, prevValue);
REGISTER_MEMBER(allMemberInfos[className], StatComponent, OnChangeStat);
REGISTER_MEMBER(allMemberInfos[className], StatComponent, OnChangeStatMap);
REGISTER_MEMBER(allMemberInfos[className], StatComponent, OnChangeStat);
REGISTER_MEMBER(allMemberInfos[className], StatComponent, nullptr);

// TextRenderComponent member registration
std::string className = typeid(TextRenderComponent).name();
REGISTER_MEMBER(allMemberInfos[className], TextRenderComponent, m_layout);
REGISTER_MEMBER(allMemberInfos[className], TextRenderComponent, m_dWriteTextFormat);
REGISTER_MEMBER(allMemberInfos[className], TextRenderComponent, m_pBrush);
REGISTER_MEMBER(allMemberInfos[className], TextRenderComponent, m_content);
REGISTER_MEMBER(allMemberInfos[className], TextRenderComponent, oss);

// TransformComponent member registration
std::string className = typeid(TransformComponent).name();
REGISTER_MEMBER(allMemberInfos[className], TransformComponent, parent);
REGISTER_MEMBER(allMemberInfos[className], TransformComponent, children);

// VideoComponent member registration
std::string className = typeid(VideoComponent).name();
REGISTER_MEMBER(allMemberInfos[className], VideoComponent, files);
REGISTER_MEMBER(allMemberInfos[className], VideoComponent, fileDirPath);
REGISTER_MEMBER(allMemberInfos[className], VideoComponent, m_bitmaps);
REGISTER_MEMBER(allMemberInfos[className], VideoComponent, bmpSize);

// Camera member registration
std::string className = typeid(Camera).name();

// gameObject member registration
std::string className = typeid(gameObject).name();
REGISTER_MEMBER(allMemberInfos[className], gameObject, m_components);
REGISTER_MEMBER(allMemberInfos[className], gameObject, createdComp);
REGISTER_MEMBER(allMemberInfos[className], gameObject, container);
REGISTER_MEMBER(allMemberInfos[className], gameObject, container);
REGISTER_MEMBER(allMemberInfos[className], gameObject, casted);
REGISTER_MEMBER(allMemberInfos[className], gameObject, nullptr);
REGISTER_MEMBER(allMemberInfos[className], gameObject, targetComponent);
REGISTER_MEMBER(allMemberInfos[className], gameObject, true);
REGISTER_MEMBER(allMemberInfos[className], gameObject, false);
REGISTER_MEMBER(allMemberInfos[className], gameObject, nullptr);
REGISTER_MEMBER(allMemberInfos[className], gameObject, m_transformComponent);

// UObject member registration
std::string className = typeid(UObject).name();
REGISTER_MEMBER(allMemberInfos[className], UObject, ClassManager);
REGISTER_MEMBER(allMemberInfos[className], UObject, objectHandle);
REGISTER_MEMBER(allMemberInfos[className], UObject, objectHandle);

// UObjectBase member registration
std::string className = typeid(UObjectBase).name();
REGISTER_MEMBER(allMemberInfos[className], UObjectBase, m_name);
REGISTER_MEMBER(allMemberInfos[className], UObjectBase, m_uuid);
