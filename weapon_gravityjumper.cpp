#include "cbase.h"
#include "weapon_gravityjumper.h"
#include "in_buttons.h"
#include "props.h"
#include "physics.h"
#include "tier0/memdbgon.h"

// Register the custom gravity jumper class
LINK_ENTITY_TO_CLASS(weapon_gravityjumper, CWeaponGravityJumper);

// Constructor
CWeaponGravityJumper::CWeaponGravityJumper()
{
    m_flNextPrimaryAttack = 0;
}

void CWeaponGravityJumper::Precache()
{
    // Precache any necessary models or sounds
    PrecacheModel("models/weapons/v_gravityjumper.mdl");
    PrecacheModel("models/weapons/w_gravityjumper.mdl");
    PrecacheScriptSound("Weapon_GravityGun.Single");
    BaseClass::Precache();
}

// Primary attack logic (gravity gun with knockback)
void CWeaponGravityJumper::PrimaryAttack()
{
    if (m_flNextPrimaryAttack > gpGlobals->curtime)
        return;

    CBasePlayer* pPlayer = ToBasePlayer(GetOwner());

    if (pPlayer)
    {
        // Perform a trace to find the object the player is targeting
        trace_t trace;
        UTIL_TraceLine(pPlayer->EyePosition(), pPlayer->EyePosition() + pPlayer->EyeVectors()->Forward() * 1500, MASK_SOLID, pPlayer, COLLISION_GROUP_PLAYER, &trace);

        // If we hit an entity (prop, player, NPC)
        if (trace.m_pEnt)
        {
            // If the hit entity is a physics object (prop or NPC), apply force
            if (trace.m_pEnt->IsPhysicsObject())
            {
                IPhysicsObject* pPhysics = trace.m_pEnt->VPhysicsGetObject();
                if (pPhysics)
                {
                    Vector vPullDirection = (pPlayer->GetAbsOrigin() - trace.m_pEnt->GetAbsOrigin());
                    vPullDirection.NormalizeInPlace();
                    pPhysics->ApplyForceCenter(vPullDirection * 500);  // Adjust the pull force
                }

                // Apply knockback to the object (20% knockback effect)
                Vector knockbackDir = trace.m_pEnt->GetAbsOrigin() - pPlayer->GetAbsOrigin();
                knockbackDir.NormalizeInPlace();
                trace.m_pEnt->ApplyAbsVelocityImpulse(knockbackDir * 200);  // Adjust for knockback strength
            }

            // If we hit a player or NPC, apply knockback directly
            if (trace.m_pEnt->IsPlayer() || trace.m_pEnt->IsNPC())
            {
                Vector knockbackDir = trace.m_pEnt->GetAbsOrigin() - pPlayer->GetAbsOrigin();
                knockbackDir.NormalizeInPlace();
                trace.m_pEnt->ApplyAbsVelocityImpulse(knockbackDir * 200);  // Adjust for knockback strength
            }
        }
        else
        {
            // If we hit a surface (ground, wall, or non-moving entity), apply knockback effect
            Vector knockbackDir = trace.endpos - pPlayer->GetAbsOrigin();
            knockbackDir.NormalizeInPlace();
            
            // If it's a wall or floor, apply a knockback force
            CBaseEntity* pHitEntity = trace.m_pEnt;
            if (pHitEntity)
            {
                // Apply a force to simulate knockback (walls or floor)
                Vector vecImpulse = knockbackDir * 200; // Adjust force as needed
                trace.m_pEnt->ApplyAbsVelocityImpulse(vecImpulse);
            }
            else
            {
                // Apply force if we hit something like the ground
                Vector vecImpulse = knockbackDir * 200;  // Apply knockback force
                pPlayer->ApplyAbsVelocityImpulse(vecImpulse);  // Apply knockback to the player if hitting a static surface
            }
        }
    }

    // Set cooldown between primary attacks
    m_flNextPrimaryAttack = gpGlobals->curtime + 0.5f;  // Adjust cooldown time (e.g., 0.5 seconds)
}

// Secondary attack (does nothing for now)
void CWeaponGravityJumper::SecondaryAttack()
{
    // No action for secondary attack (can be customized later)
}

// Weapon idle function (called when the player is not doing anything)
void CWeaponGravityJumper::WeaponIdle()
{
    // Custom idle behavior here, like animations or sounds (optional)
}
