#ifndef DUSK_TRANSFORM_DATA_INC_HLSL
#define DUSK_TRANSFORM_DATA_INC_HLSL

cbuffer DuskTransformData
{
    matrix Model;
    matrix View;
    matrix Proj;
    matrix MVP;
};

#endif // DUSK_TRANSFORM_DATA_INC_HLSL