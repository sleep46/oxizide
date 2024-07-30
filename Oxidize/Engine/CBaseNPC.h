#pragma once
#ifndef __CBASENPC_H__
#define __CBASENPC_H__

#ifndef __CCOMPONENT_H__
#include "CComponent.h"
#endif

struct AiStatistics
{
	enum FamilyEnum
	{
		Bear,
		Wolf,
		Deer,
		Boar,
		Chicken,
		Horse,
		Zombie,
		Scientist,
		Murderer,
		Player
	};

	// [Tooltip( "Ai will be less likely to fight animals that are larger than them, and more likely to flee from them." )]
	float Size;
	// [Tooltip( "How fast we can move" )]
	float Speed;
	// [Tooltip( "How fast can we accelerate" )]
	float Acceleration;
	// [Tooltip( "How fast can we turn around" )]
	float TurnSpeed;
	// [Tooltip( "Determines things like how near we'll allow other species to get" )]
	float Tolerance;
	// [Tooltip( "How far this NPC can see" )]
	float VisionRange;
	// [Tooltip( "Our vision cone for dot product - a value of -1 means we can see all around us, 0 = only infront " )]
	float VisionCone;
	// [Tooltip( "NPCs use distance visibility to basically make closer enemies easier to detect than enemies further away" )]
	void* DistanceVisibility;
	// [Tooltip( "How likely are we to be offensive without being threatened" )]
	float Hostility;
	// [Tooltip( "How likely are we to defend ourselves when attacked" )]
	float Defensiveness;
	// [Tooltip( "The range at which we will engage targets" )]
	float AggressionRange;
	// [Tooltip( "The range at which an aggrified npc will disengage it's current target" )]
	float DeaggroRange;
	// [Tooltip( "For how long will we chase a target until we give up" )]
	float DeaggroChaseTime;
	// [Tooltip( "When we deaggro, how long do we wait until we can aggro again." )]
	float DeaggroCooldown;
	// [Tooltip( "The threshold of our health fraction where there's a chance that we want to flee" )]
	float HealthThresholdForFleeing;
	// [Tooltip( "The chance that we will flee when our health threshold is triggered" )]
	float HealthThresholdFleeChance;
	// [Tooltip( "When we flee, what is the minimum distance we should flee?" )]
	float MinFleeRange;
	// [Tooltip( "When we flee, what is the maximum distance we should flee?" )]
	float MaxFleeRange;
	// [Tooltip( "When we flee, what is the maximum time that can pass until we stop?" )]
	float MaxFleeTime;
	// [Tooltip( "At what range we are afraid of a target that is in our Is Afraid Of list." )]
	float AfraidRange;
	// [Tooltip( "The family this npc belong to. Npcs in the same family will not attack each other." )]
	FamilyEnum Family;
	// [Tooltip( "List of the types of Npc that we are afraid of." )]
	FamilyEnum* IsAfraidOf;
	// [Tooltip( "The minimum distance this npc will wander when idle." )]
	float MinRoamRange;
	// [Tooltip( "The maximum distance this npc will wander when idle." )]
	float MaxRoamRange;
	// [Tooltip( "The minimum amount of time between each time we seek a new roam destination (when idle)" )]
	float MinRoamDelay;
	// [Tooltip( "The maximum amount of time between each time we seek a new roam destination (when idle)" )]
	float MaxRoamDelay;
	// [Tooltip( "If an npc is mobile, they are allowed to move when idle." )]
	bool IsMobile;
	// [Tooltip( "In the range between min and max roam delay, we evaluate the random value through this curve" )]
	void* RoamDelayDistribution;
	// [Tooltip( "For how long do we remember that someone attacked us" )]
	float AttackedMemoryTime;
	// [Tooltip( "How long should we block movement to make the wakeup animation not look whack?" )]
	float WakeupBlockMoveTime;
	// [Tooltip( "The maximum water depth this npc willingly will walk into." )]
	float MaxWaterDepth;
	// [Tooltip( "The water depth at which they will start swimming." )]
	float WaterLevelNeck;
	// [Tooltip("The water depth at which they will start swimming.")]
	float WaterLevelNeckOffset;
	// [Tooltip( "The range we consider using close range weapons." )]
	float CloseRange;
	// [Tooltip( "The range we consider using medium range weapons." )]
	float MediumRange;
	// [Tooltip( "The range we consider using long range weapons." )]
	float LongRange;
	// [Tooltip( "How long can we be out of range of our spawn point before we time out and make our way back home (when idle)." )]
	float OutOfRangeOfSpawnPointTimeout;
	// [Tooltip( "If this is set to true, then a target must hold special markers (like IsHostile) for the target to be considered for aggressive action." )]
	bool OnlyAggroMarkedTargets;
};

class CBaseNPC : public CComponent
{
public:
	void* Stats( void )
	{
		return (void*)((uintptr_t)this + 0x2A0);
	}

};

class CBaseAnimalNPC : public CBaseNPC
{
public:

};

#endif