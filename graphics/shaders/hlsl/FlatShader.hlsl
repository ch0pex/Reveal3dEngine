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
	float3 pos_l  : POSITION;
    float4 color : COLOR;
};

struct VertexOut
{
	float4 pos_h  : SV_POSITION;
    float4 color : COLOR;
};

VertexOut VS(VertexIn vin)
{
	VertexOut vout;

	float4 posW = mul(float4(vin.pos_l, 1.0f), objWorld);
	vout.pos_h = mul(posW, viewProj);
    vout.color = vin.color;
    
    return vout;
}


float4 PS(VertexOut pin) : SV_Target
{
    return pin.color;
}


