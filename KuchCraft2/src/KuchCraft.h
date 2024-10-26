///
/// @file KuchCraft.h
/// 
/// @author Michal Kuchnicki
/// 
/// @brief Header file containing the declaration of the KuchCraft game class,
///		   which encapsulates the main game logic and state.
///
/// * Is in the process of creation *

#pragma once

#include "Core/Event.h"
#include "World/World.h"

namespace KuchCraft {

	class KuchCraft
	{
	public:
		/// Constructs a new instance of the KuchCraft game.
		/// Initializes the game state and prepares necessary resources.
		KuchCraft();

		/// Destroys the KuchCraft game instance.
		/// Cleans up resources and performs any necessary shutdown procedures.
		~KuchCraft();

		/// Updates the game state.
		/// This function is called every frame and is responsible for updating the game logic
		/// based on the elapsed time.
		/// @param dt - elapsed time between frames.
		void OnUpdate(float dt);

		/// Handles input events for the game.
		/// This function processes events such as keyboard and mouse inputs, allowing
		/// the game to respond to player interactions.
		/// @param e The event to be processed.
		void OnEvent(Event& e);

		/// Executes the game logic for the current tick.
		/// This function is called to perform fixed updates, allowing for consistent 
		/// game mechanics that are independent of the frame rate.
		void OnTick();

		std::shared_ptr<World> GetWorld() { return m_World; }

	private:
		std::shared_ptr<World> m_World;

	};

}