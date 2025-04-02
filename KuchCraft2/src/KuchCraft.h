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
#include "World/World/World.h"

namespace KuchCraft {

	enum class KuchCraftState
	{
		/// Represents the main menu state of the game, where players can create, select, or delete worlds.
		MainMenu,

		/// Represents the active gameplay state, where the world is updated, and player actions are processed.
		InGame,

		/// Represents the paused state during gameplay, where no active updates to the world occur.
		InGamePaused
	};

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

		/// Updates the main menu logic.
		/// This method handles user interactions and UI rendering specific to the main menu.
		/// @param dt - elapsed time between frames.
		void OnUpdateMainMenu(float dt);

		/// Updates the in-game logic.
		/// This method handles gameplay mechanics, world updates, and player interactions during active gameplay.
		/// @param dt - elapsed time between frames.
		void OnUpdateInGame(float dt);

		/// Updates the paused game logic.
		/// This method manages UI interactions and ensures gameplay is halted while in the paused state.
		/// @param dt - elapsed time between frames.
		void OnUpdateInGamePaused(float dt);

		/// Renders ImGui-specific components for the KuchCraft.
		/// This method handles the application's custom ImGui rendering logic.
		void OnImGuiRender();

		/// Handles input events for the game.
		/// This function processes events such as keyboard and mouse inputs, allowing
		/// the game to respond to player interactions.
		/// @param e The event to be processed.
		void OnEvent(Event& e);

		/// Handles key press events.
		/// This function is called when a key is pressed, and it interprets user commands.
		/// @param e The key pressed event to be processed.
		/// @return True if the event is handled, false otherwise.
		bool OnKeyPressed(KeyPressedEvent& e);

		/// Shows or hides the in-game cursor. 
		/// TODO: Add in game cursor, currently uses windows cursor and can get bugs
		/// This method toggles the visibility of the cursor depending on the provided status.
		/// @param status True to show the cursor, false to hide it.
		void ShowInGameCursor(bool status);

		/// Changes the state of the game.
		/// This method transitions the game between different states such as MainMenu, InGame, and InGamePaused.
		/// @param state The new state to transition to.
		void ChangeState(KuchCraftState state);

	private:
		/// Stores the current game world.
		std::shared_ptr<World> m_World;

		/// Represents the current state of the game.
		/// This variable is used to determine the active state and behavior of the game logic.
		KuchCraftState m_State;

	};

}