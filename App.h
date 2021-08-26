#pragma once
#include "AppIncl.h"

void Start();
void Exit();

void Update(float dt);

void OnKeyPressed(LPARAM lParam);
void OnKeyReleased(LPARAM lParam);

void OnMouseButtonPressed(LPARAM lParam);
void OnMouseButtonReleased(LPARAM lParam);