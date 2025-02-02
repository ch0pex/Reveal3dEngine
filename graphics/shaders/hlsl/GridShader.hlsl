

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

struct VertexIn
{
    uint vertexId : SV_VertexID;
};

struct VertexOut
{
    float4 pos : SV_Position;
    float3 nearPoint: NEAR_POINT;
    float3 farPoint: FAR_POINT;
//     float4 color : color;
};

struct FragOut
{
    float4 color: SV_Target;
    float depth: SV_Depth;
};

static const float3 gridPlane[6] = {
    float3(1, 1, 0), float3(-1, -1, 0), float3(-1, 1, 0),
    float3(-1, -1, 0), float3(1, 1, 0), float3(1, -1, 0)
};

float4x4 inverse(float4x4 m) {
    float n11 = m[0][0], n12 = m[1][0], n13 = m[2][0], n14 = m[3][0];
    float n21 = m[0][1], n22 = m[1][1], n23 = m[2][1], n24 = m[3][1];
    float n31 = m[0][2], n32 = m[1][2], n33 = m[2][2], n34 = m[3][2];
    float n41 = m[0][3], n42 = m[1][3], n43 = m[2][3], n44 = m[3][3];

    float t11 = n23 * n34 * n42 - n24 * n33 * n42 + n24 * n32 * n43 - n22 * n34 * n43 - n23 * n32 * n44 + n22 * n33 * n44;
    float t12 = n14 * n33 * n42 - n13 * n34 * n42 - n14 * n32 * n43 + n12 * n34 * n43 + n13 * n32 * n44 - n12 * n33 * n44;
    float t13 = n13 * n24 * n42 - n14 * n23 * n42 + n14 * n22 * n43 - n12 * n24 * n43 - n13 * n22 * n44 + n12 * n23 * n44;
    float t14 = n14 * n23 * n32 - n13 * n24 * n32 - n14 * n22 * n33 + n12 * n24 * n33 + n13 * n22 * n34 - n12 * n23 * n34;

    float det = n11 * t11 + n21 * t12 + n31 * t13 + n41 * t14;
    float idet = 1.0f / det;

    float4x4 ret;

    ret[0][0] = t11 * idet;
    ret[0][1] = (n24 * n33 * n41 - n23 * n34 * n41 - n24 * n31 * n43 + n21 * n34 * n43 + n23 * n31 * n44 - n21 * n33 * n44) * idet;
    ret[0][2] = (n22 * n34 * n41 - n24 * n32 * n41 + n24 * n31 * n42 - n21 * n34 * n42 - n22 * n31 * n44 + n21 * n32 * n44) * idet;
    ret[0][3] = (n23 * n32 * n41 - n22 * n33 * n41 - n23 * n31 * n42 + n21 * n33 * n42 + n22 * n31 * n43 - n21 * n32 * n43) * idet;

    ret[1][0] = t12 * idet;
    ret[1][1] = (n13 * n34 * n41 - n14 * n33 * n41 + n14 * n31 * n43 - n11 * n34 * n43 - n13 * n31 * n44 + n11 * n33 * n44) * idet;
    ret[1][2] = (n14 * n32 * n41 - n12 * n34 * n41 - n14 * n31 * n42 + n11 * n34 * n42 + n12 * n31 * n44 - n11 * n32 * n44) * idet;
    ret[1][3] = (n12 * n33 * n41 - n13 * n32 * n41 + n13 * n31 * n42 - n11 * n33 * n42 - n12 * n31 * n43 + n11 * n32 * n43) * idet;

    ret[2][0] = t13 * idet;
    ret[2][1] = (n14 * n23 * n41 - n13 * n24 * n41 - n14 * n21 * n43 + n11 * n24 * n43 + n13 * n21 * n44 - n11 * n23 * n44) * idet;
    ret[2][2] = (n12 * n24 * n41 - n14 * n22 * n41 + n14 * n21 * n42 - n11 * n24 * n42 - n12 * n21 * n44 + n11 * n22 * n44) * idet;
    ret[2][3] = (n13 * n22 * n41 - n12 * n23 * n41 - n13 * n21 * n42 + n11 * n23 * n42 + n12 * n21 * n43 - n11 * n22 * n43) * idet;

    ret[3][0] = t14 * idet;
    ret[3][1] = (n13 * n24 * n31 - n14 * n23 * n31 + n14 * n21 * n33 - n11 * n24 * n33 - n13 * n21 * n34 + n11 * n23 * n34) * idet;
    ret[3][2] = (n14 * n22 * n31 - n12 * n24 * n31 - n14 * n21 * n32 + n11 * n24 * n32 + n12 * n21 * n34 - n11 * n22 * n34) * idet;
    ret[3][3] = (n12 * n23 * n31 - n13 * n22 * n31 + n13 * n21 * n32 - n11 * n23 * n32 - n12 * n21 * n33 + n11 * n22 * n33) * idet;

    return ret;
}

float3 UnprojectPoint(float x, float y, float z, float4x4 viewProj)
{
    float4x4 viewProjInv = inverse(viewProj);
    float4 unprojectedPoint =  mul(float4(x, y, z, 1.0), viewProjInv);
    return unprojectedPoint.xyz / unprojectedPoint.w;
}

float4 grid(float3 fragPos3D, float scale, float3 gridColor) {
    float2 coord = fragPos3D.xy * scale; // use the scale variable to set the distance between the lines
    float2 derivative = fwidth(coord);
    float2 grid = abs(frac(coord - 0.5) - 0.5) / derivative;
    float l = min(grid.x, grid.y);
    float minimumz = min(derivative.y, 1);
    float minimumx = min(derivative.x, 1);
    float4 color = float4(gridColor, 1.0 - min(l, 1.0));
    // z axis
    if(fragPos3D.x > -0.1 * minimumx && fragPos3D.x < 0.1 * minimumx)
        color.z = 1.0;
    // x axis
    if(fragPos3D.y > -0.1 * minimumz && fragPos3D.y < 0.1 * minimumz)
        color.x = 1.0;
    return color;
}

float computeDepth(float3 pos) {
    float4 clip_space_pos = mul(float4(pos.xyz, 1.0), viewProj);
    return (clip_space_pos.z / clip_space_pos.w);
}

float computeLinearDepth(float3 pos) {
  float4 clip_space_pos =  mul(float4(pos.xyz, 1.0), viewProj);
  float clip_space_depth = (clip_space_pos.z / clip_space_pos.w) * 2.0 - 1.0; // put back between -1 and 1
  float linearDepth = (2.0 * nearZ * farZ) / (farZ + nearZ - clip_space_depth * (farZ - nearZ)); // get linear value between 0.01 and 100
  return linearDepth / farZ; // normalizealize
}

VertexOut VS(VertexIn input)
{
    VertexOut output;
    float3 p = gridPlane[input.vertexId];
    output.pos =float4(p, 1.0f);
    output.farPoint = UnprojectPoint(p.x, p.y, 1.0, viewProj).xyz; // unprojecting on the far plane
    output.nearPoint =  UnprojectPoint(p.x, p.y, 0.0, viewProj).xyz;

    return output;
}

FragOut PS(VertexOut pin)
{
    FragOut output;
    float t = -pin.nearPoint.z / (pin.farPoint.z - pin.nearPoint.z);
    float3 fragPos3D = pin.nearPoint + t * (pin.farPoint - pin.nearPoint);
    output.depth = computeDepth(fragPos3D);
//     output.color = mul(grid(fragPos3D, 2), float(t > 0));

    float linearDepth = computeLinearDepth(fragPos3D);
    float fading = max(0, (0.5 - linearDepth));

    output.color = (grid(fragPos3D, 2, float3(0.1f, 0.1f, 0.1f)) + grid(fragPos3D, 1, float3(0.5f, 0.5f, 0.5f)))* float(t > 0); // adding multiple resolution for the grid
    output.color.a *= fading;

    return output;
}
