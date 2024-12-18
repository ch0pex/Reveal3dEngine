/************************************************************************
 * Copyright (c) 2024 Alvaro Cabrera Barrio
 * This code is licensed under MIT license (see LICENSE.txt for details)
 ************************************************************************/

cbuffer cbPass : register(b2)
{
    float4x4    view;
    float4x4    invView;
    float4x4    proj;
    float4x4    invProj;
    float4x4    viewProj;
    float4x4    invViewProj;
    float2      cbPerObjectPad1;
    float2      renderTargetSize;
    float3      eyePos;
    float       nearZ;
    float       farZ;
    float       totalTime;
    float       deltaTime;
};

cbuffer cbMaterial : register(b1)
{
	float4      baseDiffuse;
	float3      fresnelR0;
	float       roughness;
	float4x4    matTransform;
};

cbuffer cbPerObject : register(b0)
{
	float4x4 objWorld;
	uint id;
};

struct VertexIn
{
	float3 pos_l    : POSITION;
    float3 normal_l : NORMAL;
//    float4 color   : COLOR;
//    float2 texcoord : TEXCOORD0;
};

struct VertexOut
{
    float3 pos_w : POSITION;
	float4 pos_h  : SV_POSITION;
    float3 normal_w : NORMAL;
};


VertexOut VS(VertexIn vin)
{
	VertexOut vout;
	float4 posW = mul(float4(vin.pos_l, 1.0f), objWorld);

  vout.pos_w = posW.xyz;
  vout.pos_h = mul(posW, viewProj);
  vout.normal_w = mul(vin.normal_l, (float3x3)objWorld);

  return vout;
}

float4 PS(VertexOut pin) : SV_Target
{
    float sunLightIntensity = 0.6;
    float3 sunLightDir = float3(0.0f, 0.0f, -1.0f);
    float4 sunLightcolor = float4(1.0f, 1.0f, 1.0f, 0.0f);
   	float3 lightDir = -normalize(sunLightDir);

    // Ambient lighting
    float ambientLightIntensity = 0.7f;
    float4 ambientcolor = float4(1.0f, 1.0f, 1.0f, 0.0f);
   	float4 ambientLight =  mul(ambientcolor, ambientLightIntensity);

    // Diffuse lighting
   	float3 norm = normalize(pin.normal_w);
   	float diff = max(dot(norm, lightDir), 0.0f);
   	float4 diffuseLight = sunLightcolor * diff  * sunLightIntensity;

    // Specular lighting
    float3 viewDir = normalize(pin.pos_w - eyePos);
    float3 reflectDir = reflect(lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32) ;
    float4 specularLight = spec * sunLightIntensity * sunLightcolor;

    return baseDiffuse * (ambientLight + diffuseLight + specularLight);
}


