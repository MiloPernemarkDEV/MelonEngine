RWTexture2D<float4> image : register(u0);

[numthreads(16, 16, 1)]
void CSmain(
    uint3 dispatchID : SV_DispatchThreadID,
    uint3 groupID    : SV_GroupThreadID)
{
    uint2 texelCoord = dispatchID.xy;

    uint x, y;
    image.GetDimensions(x, y);

    if (texelCoord.x < x && texelCoord.y < y)
    {
        float4 color = float4(0.0, 0.0, 0.0, 1.0);

        if (groupID.x != 0 && groupID.y != 0)
        {
            color.x = float(texelCoord.x) / float(x);
            color.y = float(texelCoord.y) / float(y);
        }

        image[texelCoord] = color;
    }
}