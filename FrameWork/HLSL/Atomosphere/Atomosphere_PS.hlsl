//================================================
//
//大気散乱
//
//================================================
#include "../Common/Common.hlsl"


// 交差ポジションを求める
float3 IntersectionPos(float3 rayPos, float3 rayDir, float sphereRadius)
{
	const float A = dot(rayDir, rayDir);
	const float B = 2.0*dot(rayPos, rayDir);
	const float C = dot(rayPos, rayPos) - sphereRadius * sphereRadius;

	return B * B - 4.0 * A * C < 0.000001 ? float3(0, 0, 0) : (rayPos + rayDir * (0.5 * (-B + sqrt(B * B - 4.0 * A * C)) / A));
}

// 積分近似値を求める
float IntegralApproximation(float fCos)
{
	float x = 1.0 - fCos;
	return fScaleDepth * exp(-0.00287 + x * (0.459 + x * (3.83 + x * (-6.80 + x * 5.25))));
}

float4 PS(in PS_IN input) : SV_Target
{
	float2 uv = (input.Position.xy / float2(windowSize));

	float3 deltaX = (frustumRayTR - frustumRayTL) * uv.x;
	float3 deltaY = (frustumRayBL - frustumRayTL) * uv.y;
	float3 ray = normalize((frustumRayTL + deltaX + deltaY).xyz);

	float3 skyPos = IntersectionPos(CameraPosition.xyz, ray, fOuterRadius);
	if (skyPos.x == 0)
	{
		//Camera is out of celestial sphere.
		clip(-1);
	}

	float3 invWavelength = v3InvWavelength;

	float3 v3Pos = skyPos;
	float3 v3Ray = v3Pos - CameraPosition.xyz;
	float fFar = length(v3Ray);
	v3Ray /= fFar;

	// Calculate the ray's starting position, then calculate its scattering offset
	float3 v3Start = CameraPosition;
	float fHeight = length(v3Start);
	float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - CameraLength));
	float fStartAngle = dot(v3Ray, v3Start) / fHeight;
	float fStartOffset = fDepth * IntegralApproximation(fStartAngle);

	const int nSamples = 5;
	const float fSamples = nSamples;

	// Initialize the scattering loop variables
	float fSampleLength = fFar / fSamples;
	float fScaledLength = fSampleLength * fScale;
	float3 v3SampleRay = v3Ray * fSampleLength;
	float3 v3SamplePoint = v3Start + v3SampleRay * 0.5;

	// Now loop through the sample rays
	float3 v3FrontColor = float3(0.0, 0.0, 0.0);
	for (int i = 0; i < nSamples; ++i)
	{
		float fHeight = length(v3SamplePoint);
		float fDepth = exp(fScaleOverScaleDepth * (fInnerRadius - fHeight));
		float fLightAngle = dot(v3LightPos, v3SamplePoint) / fHeight;
		float fCameraAngle = dot(v3Ray, v3SamplePoint) / fHeight;
		float fScatter = (fStartOffset + fDepth * (IntegralApproximation(fLightAngle) - IntegralApproximation(fCameraAngle)));
		float3 v3Attenuate = exp(-fScatter * (invWavelength * fKr4PI + fKm4PI));
		v3FrontColor += v3Attenuate * (fDepth * fScaledLength); // これに関する値が不具合を起こしてる
		v3SamplePoint += v3SampleRay;
	}

	float4 primaryColor = float4(v3FrontColor * (invWavelength * fKrESun), 1.0);
	float4 secondaryColor = float4(v3FrontColor * fKmESun, 1.0);
	float3 v3Direction = CameraPosition - v3Pos;

	float fCos = dot(v3LightPos, v3Direction) / length(v3Direction);
	float fRayPhase = 0.75 * (1.0 + fCos * fCos);

	const float g2 = g * g;
	float fMiePhase = 1.5 * ((1.0 - g2) / (2.0 + g2)) * (1.0 + fCos * fCos) / pow(abs(1.0 + g2 - 2.0*g*fCos), 1.5);

	float3 raycolor = (primaryColor*fRayPhase).xyz;
	float3 miecolor = (secondaryColor*fMiePhase).xyz;

	float3 c = float3(1.0, 1.0, 1.0) - exp(-exposure * (raycolor + miecolor));
	return float4(saturate(c), 1.0);
}