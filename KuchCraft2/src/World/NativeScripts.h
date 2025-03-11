#pragma once

#include "KuchCraft/CameraController.h"
#include "KuchCraft/MovableObject.h"
#include "KuchCraft/Player.h"

namespace KuchCraft {

	using AllNativeScripts = ComponentGroup<CameraController, MovableObject, Player>;

}