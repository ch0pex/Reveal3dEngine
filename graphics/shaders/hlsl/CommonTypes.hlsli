
//TODO: Use compiling tools, instead of runtime compilation

cbuffer cbPerObject : register(b0)
{
	float4x4 objWorld;
};

cbuffer cbMaterial : register(b1)
{
	float4      baseDiffuse;
	float3      fresnelR0;
	float       roughness;
	float4x4    matTransform;
};

cbuffer cbPass : register(b2)
{
    float4x4 view;
    float4x4 invView;
    float4x4 proj;
    float4x4 invProj;
    float4x4 viewProj;
    float4x4 invViewProj;
    float3 eyePos;
    float2 cbPerObjectPad1;
    float2 renderTargetSize;
    float nearZ;
    float farZ;
    float totalTime;
    float deltaTime;
};
