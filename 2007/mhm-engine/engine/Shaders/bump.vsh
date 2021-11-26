//========================================================
// vertex shader constants table
//========================================================
float4x4 mxCamera;
float4x4 mxWorld;
float4   vLightA;
float4   vLightB;
float4   vViewDirection;

//========================================================
// vertex shader input semantics
//========================================================
struct VS_INPUT_STRUCT
{
	float4 position: POSITION;
	float3 normal:	 NORMAL;
	float2 texcoord: TEXCOORD0;
	float3 tangent:  TANGENT;
};

//========================================================
// vertex shader output semantics
//========================================================
struct VS_OUTPUT_STRUCT
{
	float4 position: POSITION;
	float2 texcoord: TEXCOORD0;
	float3 normal:   TEXCOORD1;
	float3 lightA:   TEXCOORD2;
	float3 viewdir:  TEXCOORD3;
	float3 halfang:  TEXCOORD4;	
};

//========================================================
// vertex shader program
//========================================================
VS_OUTPUT_STRUCT main (VS_INPUT_STRUCT In)
{
	VS_OUTPUT_STRUCT Out;
	
	float4 TPosition = mul(mxWorld, In.position);
	Out.normal       = normalize(mul(mxWorld, In.normal));
	Out.position     = mul(TPosition, mxCamera);	
	Out.texcoord     = In.texcoord;
	float3 vBinormal = cross(In.normal, In.tangent);
	float3x3 mxTangent;
	mxTangent[0] = mul(mxWorld, In.tangent);
	mxTangent[1] = mul(mxWorld, vBinormal);
	mxTangent[2] = Out.normal;
	float3 light = normalize(vLightA - TPosition);
	float3 viewdir = normalize(vViewDirection - TPosition);
	Out.lightA   = mul(mxTangent, light);
	Out.viewdir  = mul(mxTangent, viewdir);
	Out.halfang  = normalize((Out.viewdir + Out.lightA) / 2.0f);
	return Out;
}




