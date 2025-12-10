/*
 *  Filename: Rigidbody2D.cpp
 *  This code is part of the Nth Engine core library
 *  Copyright 2025 Jake Rieger
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *  In no event and under no legal theory, whether in tort (including negligence),
 *  contract, or otherwise, unless required by applicable law (such as deliberate
 *  and grossly negligent acts) or agreed to in writing, shall any Contributor be
 *  liable for any damages, including any direct, indirect, special, incidental,
 *  or consequential damages of any character arising as a result of this License or
 *  out of the use or inability to use the software (including but not limited to damages
 *  for loss of goodwill, work stoppage, computer failure or malfunction, or any and
 *  all other commercial damages or losses), even if such Contributor has been advised
 *  of the possibility of such damages.
 */

#include "Rigidbody2D.hpp"

namespace Nth {
    Rigidbody2D::Rigidbody2D() {
        UpdateMass(mass);
    }

    void Rigidbody2D::UpdateMass(f32 newMass) {
        mass = newMass;
        if (type == BodyType::Static) {
            inverseMass    = 0.0f;
            inverseInertia = 0.0f;
        } else {
            inverseMass = (mass > 0.0f) ? 1.0f / mass : 0.0f;
            // Simple inertia approximation for a point mass
            // This should be recalculated based on collider shape
            inertia        = mass;
            inverseInertia = (inertia > 0.0f) ? 1.0f / inertia : 0.0f;
        }
    }

    void Rigidbody2D::ApplyForce(const Vec2& f) {
        if (type != BodyType::Dynamic) return;
        force += f;
    }

    void Rigidbody2D::ApplyImpulse(const Vec2& impulse) {
        if (type != BodyType::Dynamic) return;
        velocity += impulse * inverseMass;
    }

    void Rigidbody2D::ApplyImpulseAtPoint(const Vec2& impulse, const Vec2& contactPoint, const Vec2& centerOfMass) {
        if (type != BodyType::Dynamic) return;

        // Linear impulse
        velocity += impulse * inverseMass;

        // Angular impulse
        if (!lockRotation) {
            const Vec2 r     = contactPoint - centerOfMass;
            const auto cross = r.x * impulse.y - r.y * impulse.x;
            angularVelocity += cross * inverseInertia;
        }
    }

    void Rigidbody2D::ApplyTorque(f32 t) {
        if (type != BodyType::Dynamic || lockRotation) return;
        torque += t;
    }

    void Rigidbody2D::ClearForces() {
        force  = Vec2(0.0f);
        torque = 0.0f;
    }
}  // namespace Nth