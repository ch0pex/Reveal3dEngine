/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/

cbuffer cbPass : register(b1)
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

cbuffer cbPerObject : register(b0)
{
	float4x4 objWorld;
};

struct VertexIn
{
	float3 PosL    : POSITION;
    float4 Color   : COLOR;
    float3 NormalL : NORMAL;
};

struct VertexOut
{
    float3 PosW : POSITION;
	float4 PosH  : SV_POSITION;
    float4 Color : COLOR;
    float3 NormalW : NORMAL;
};


VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	float4 posW = mul(float4(vin.PosL, 1.0f), objWorld);

	vout.PosH = mul(posW, viewProj);
    vout.NormalW = mul(vin.NormalL, (float3x3)objWorld);
    vout.Color = vin.Color;

    return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float sunLightIntensity = 0.8;
    float3 sunLightDir = float3(0.0f, 1.0f, 0.0f);
    float4 sunLightColor = float4(1.0f, 1.0f, 1.0f, 0.0f);

    float ambientLightIntensity = 0.5f;
    float4 ambientColor = float4(1.0f, 1.0f, 1.0f, 0.0f);
   	float4 ambientLight =  mul(ambientColor, ambientLightIntensity);

   	float3 norm = normalize(pin.NormalW);
   	float3 lightDir = normalize(sunLightDir);
   	float diff = max(dot(norm, lightDir), 0.0f);
   	float4 diffuseLight = mul(mul(sunLightColor, diff), sunLightIntensity);

    return pin.Color * (ambientLight + diffuseLight);
}


