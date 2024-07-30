#pragma once
#ifndef __CMODEL_H__
#define __CMODEL_H__

#include "..\includes.h"

#ifndef __GAME_H__
#include "Game\Game.h"
#endif

#ifndef __CCOMPONENT_H__
#include "CComponent.h"
#endif

class CModel : public CComponent
{
public:
	static void OnGameLeave( void ) 
	{ 
	}

public:

	Vector3 GetBonePosition( BoneList BoneID )
	{
		return GetBone( GetTransform( BoneID ) );//GetBone
	}

	Vector3 GetBone( uintptr_t pTransform )
	{
		__m128 result {};

		const __m128 mulVec0 = { -2.000, 2.000, -2.000, 0.000 };
		const __m128 mulVec1 = { 2.000, -2.000, -2.000, 0.000 };
		const __m128 mulVec2 = { -2.000, -2.000, 2.000, 0.000 };

		auto pTransformAccessReadOnly = ReadPhysMemory<TransformAccessReadOnly>( pTransform + 0x38 );
		auto transformData = ReadPhysMemory<TransformData>( pTransformAccessReadOnly.pTransformData + 0x18 );

		unsigned int index = ReadPhysMemory<unsigned int>( pTransform + 0x40 );

		if (transformData.pTransformArray && transformData.pTransformIndices)
		{
			result = ReadPhysMemory< __m128>( transformData.pTransformArray + 0x30 * index);
			int transformIndex = ReadPhysMemory<int>( transformData.pTransformIndices + 0x4 * index );

			int pSafe = 0;
			while (transformIndex >= 0 && pSafe++ < 200)
			{
				Matrix3x4 matrix34 = ReadPhysMemory<Matrix3x4>( transformData.pTransformArray + 0x30 * transformIndex );

				__m128 xxxx = _mm_castsi128_ps( _mm_shuffle_epi32( *(__m128i*)(&matrix34.vec1), 0x00 ) );	// xxxx
				__m128 yyyy = _mm_castsi128_ps( _mm_shuffle_epi32( *(__m128i*)(&matrix34.vec1), 0x55 ) );	// yyyy
				__m128 zwxy = _mm_castsi128_ps( _mm_shuffle_epi32( *(__m128i*)(&matrix34.vec1), 0x8E ) );	// zwxy
				__m128 wzyw = _mm_castsi128_ps( _mm_shuffle_epi32( *(__m128i*)(&matrix34.vec1), 0xDB ) );	// wzyw
				__m128 zzzz = _mm_castsi128_ps( _mm_shuffle_epi32( *(__m128i*)(&matrix34.vec1), 0xAA ) );	// zzzz
				__m128 yxwy = _mm_castsi128_ps( _mm_shuffle_epi32( *(__m128i*)(&matrix34.vec1), 0x71 ) );	// yxwy
				__m128 tmp7 = _mm_mul_ps( *(__m128*)(&matrix34.vec2), result );

				result = _mm_add_ps(
					_mm_add_ps(
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps( _mm_mul_ps( xxxx, mulVec1 ), zwxy ),
									_mm_mul_ps( _mm_mul_ps( yyyy, mulVec2 ), wzyw ) ),
								_mm_castsi128_ps( _mm_shuffle_epi32( _mm_castps_si128( tmp7 ), 0xAA ) ) ),
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps( _mm_mul_ps( zzzz, mulVec2 ), wzyw ),
									_mm_mul_ps( _mm_mul_ps( xxxx, mulVec0 ), yxwy ) ),
								_mm_castsi128_ps( _mm_shuffle_epi32( _mm_castps_si128( tmp7 ), 0x55 ) ) ) ),
						_mm_add_ps(
							_mm_mul_ps(
								_mm_sub_ps(
									_mm_mul_ps( _mm_mul_ps( yyyy, mulVec0 ), yxwy ),
									_mm_mul_ps( _mm_mul_ps( zzzz, mulVec1 ), zwxy ) ),
								_mm_castsi128_ps( _mm_shuffle_epi32( _mm_castps_si128( tmp7 ), 0x00 ) ) ),
							tmp7 ) ), *(__m128*)(&matrix34.vec0) );

				transformIndex = ReadPhysMemory<int>( transformData.pTransformIndices + 0x4 * transformIndex );
			}
		}

		return Vector3( result.m128_f32[ 0 ], result.m128_f32[ 1 ], result.m128_f32[ 2 ] );
	}
	DWORD64 GetTransform( int bone )
	{
		DWORD64 boneTransforms = GetValue<DWORD64>( 0x48 );//public Transform[] boneTransforms;
		DWORD64 BoneValue = ReadPhysMemory<DWORD64>( (boneTransforms + (0x20 + (bone * 0x8))) );
		return ReadPhysMemory<DWORD64>( BoneValue + 0x10 );
	}
};


#endif