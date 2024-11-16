#ifndef WEAPON_GRAVITYJUMPER_H
#define WEAPON_GRAVITYJUMPER_H

#include "cbase.h"
#include "weapon_physicsgun.h"  // Inherit from the Physics Gun class

// Define the custom gravity jumper weapon class
class CWeaponGravityJumper : public CWeaponPhysicsGun
{
    DECLARE_CLASS(CWeaponGravityJumper, CWeaponPhysicsGun);

public:
    // Constructor
    CWeaponGravityJumper();

    // Precache necessary models and sounds
    void Precache() override;

    // Primary attack logic
    void PrimaryAttack() override;

    // Secondary attack logic (not used here, can be expanded)
    void SecondaryAttack() override;

    // Idle behavior
    void WeaponIdle() override;

private:
    // Store the next time the primary attack can be performed (cooldown)
    float m_flNextPrimaryAttack;
};

#endif // WEAPON_GRAVITYJUMPER_H
