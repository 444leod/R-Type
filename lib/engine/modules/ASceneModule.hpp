/*
** EPITECH PROJECT, 2024
** R-Type
** File description:
** ASceneModule
*/

#pragma once

class AScene;

class ASceneModule {
public:
    explicit ASceneModule(AScene& scene): _scene(scene) {}
    virtual ~ASceneModule() = default;

protected:
    AScene& _scene;
};
