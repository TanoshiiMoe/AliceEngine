// ��ƼŬ ���� �� �ռ� ���̴�
// ���� ���̾� ����, �۷ο�, �׸��� ȿ��

struct PSInput
{
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
    float time : TEXCOORD1;
    float lifeRatio : TEXCOORD2;
    float2 worldPos : TEXCOORD3;
    float depth : TEXCOORD4;
};

// ���̴� ��� ����
cbuffer BlendingConstants : register(b0)
{
    float4 blendData;       // x: blendMode, y: glowIntensity, z: shadowStrength, w: layerOpacity
    float4 timeData;        // x: time, y: deltaTime, z: emissionTime, w: unused
    float4 effectData;      // x: bloomThreshold, y: bloomScale, z: dofFocus, w: dofRange
    float4 transformData;   // x: scale, y: rotation, z: unused, w: unused
};

// ���� ��� �Լ���
float3 blendAdd(float3 base, float3 blend)
{
    return min(base + blend, 1.0);
}

float3 blendMultiply(float3 base, float3 blend)
{
    return base * blend;
}

float3 blendScreen(float3 base, float3 blend)
{
    return 1.0 - (1.0 - base) * (1.0 - blend);
}

float3 blendOverlay(float3 base, float3 blend)
{
    return lerp(
        2.0 * base * blend,
        1.0 - 2.0 * (1.0 - base) * (1.0 - blend),
        step(0.5, base)
    );
}

float3 blendSoftLight(float3 base, float3 blend)
{
    return lerp(
        2.0 * base * blend + base * base * (1.0 - 2.0 * blend),
        sqrt(base) * (2.0 * blend - 1.0) + 2.0 * base * (1.0 - blend),
        step(0.5, blend)
    );
}

// �۷ο� ȿ��
float3 applyGlow(float3 color, float intensity, float threshold)
{
    float luminance = dot(color, float3(0.299, 0.587, 0.114));
    float glow = smoothstep(threshold, threshold + 0.1, luminance);
    return color + glow * intensity * float3(1.0, 0.8, 0.6);
}

// ��� ȿ��
float3 applyBloom(float3 color, float threshold, float scale)
{
    float luminance = dot(color, float3(0.299, 0.587, 0.114));
    float bloom = smoothstep(threshold, threshold + 0.2, luminance);
    return color + bloom * scale * color;
}

// ���̿� ���� �� ȿ�� (DOF)
float calculateDOFBlur(float depth, float focus, float range)
{
    float distance = abs(depth - focus);
    return smoothstep(0.0, range, distance);
}

// ���� �ȼ� ���̴�
float4 D2DMain(PSInput input) : SV_TARGET
{
    float time = timeData.x;
    float lifeRatio = input.lifeRatio;
    
    // �⺻ ����
    float4 baseColor = input.color;
    
    // ���� ��� ����
    float3 blendedColor = baseColor.rgb;
    int blendMode = (int) blendData.x;
    
    // ���� ���� ���� ��ȭ
    float3 lifeColor = lerp(float3(1.0, 0.3, 0.0), float3(0.0, 0.7, 1.0), lifeRatio);
    
    
    if (blendMode == 0)
    {
        blendedColor = blendAdd(blendedColor, lifeColor * 0.3);
    }
    else if (blendMode == 1)
    {
        blendedColor = blendMultiply(blendedColor, lifeColor);
    }
    else if (blendMode == 2)
    {
        blendedColor = blendScreen(blendedColor, lifeColor * 0.5);
    }
    else if (blendMode == 3)
    {
        blendedColor = blendOverlay(blendedColor, lifeColor);
    }
    else if (blendMode == 4)
    {
        blendedColor = blendSoftLight(blendedColor, lifeColor);
    }
    else
    {
        blendedColor = blendAdd(blendedColor, lifeColor * 0.2);
    }
    
    // �۷ο� ȿ��
    float glowIntensity = blendData.y * (1.0 - lifeRatio);
    blendedColor = applyGlow(blendedColor, glowIntensity, 0.7);
    
    // ��� ȿ��
    float bloomThreshold = effectData.x;
    float bloomScale = effectData.y;
    blendedColor = applyBloom(blendedColor, bloomThreshold, bloomScale);
    
    // �׸��� ȿ��
    float shadowStrength = blendData.z;
    float shadow = smoothstep(0.0, 0.3, lifeRatio);
    blendedColor *= lerp(0.3, 1.0, shadow);
    
    // ���̿� ���� �� (DOF)
    float dofFocus = effectData.z;
    float dofRange = effectData.w;
    float dofBlur = calculateDOFBlur(input.depth, dofFocus, dofRange);
    
    // �ð��� ���� ���� ��ȭ
    float timeShift = sin(time * 2.0) * 0.1;
    blendedColor += timeShift * float3(0.1, 0.05, 0.2);
    
    // ���̾� ����
    float layerOpacity = blendData.w;
    float finalAlpha = baseColor.a * lifeRatio * layerOpacity;
    
    // ���� ���� ��ȯ
    return float4(blendedColor, finalAlpha);
}