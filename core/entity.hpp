/************************************************************************
* Copyright (c) 2024 Alvaro Cabrera Barrio
* This code is licensed under MIT license (see LICENSE.txt for details)
************************************************************************/
/**
* @file entity.hpp
* @version 1.0
* @date 15/07/2024
* @brief Short description
*
* Longer description
*/

#include "common/common.hpp" 

namespace reveal3d::core {

class Entity {
public:
    Entity() : id_(id::invalid) {}
    explicit Entity(id_t id);

    // std::string& Name() const;
    // Transform& Transform();
    // Geometry& Geometry();
    // Script Script();

    template<typename T> T& Component();
    template<typename T> void AddComponent();

    INLINE u32 Id() const { return id_; }
    bool IsAlive();

private:
    void GenerateId();
    id_t id_;
};

template<typename T>
inline void Entity::AddComponent() {
    
}

template <typename T>
inline T &Entity::GetComponent()
{
    // TODO: Insertar una instrucción "return" aquí
}

}