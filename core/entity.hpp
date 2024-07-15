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

class Entity {
public:
    Entity() : id_(id::invalid) {}
    explicit Entity(std::string& name);
    explicit Entity(const wchar_t *path);
    explicit Entity(id_t id);

    std::string& Name() const;
    Transform& Transform();
    Geometry& Geometry();
    Script Script();

    template<typename T> T& GetComponent();
    template<typename T> void AddComponent();

    void SetName(std::string_view name);

    INLINE u32 Id() const { return id_; }
    bool IsAlive();

private:
    void GenerateId();
    id_t id_;
};
