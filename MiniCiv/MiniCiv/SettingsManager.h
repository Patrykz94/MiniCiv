#pragma once

#include <SFML/Graphics.hpp>

#include <map>
#include <string>
#include <assert.h>

class SettingsManager {
public:
	SettingsManager();
	~SettingsManager() {}

	/////////////////////////////////////////////////////////
	
	// Returns string "fullscreen",
	// "window" or "borderlesswindow"
	std::string GetScreenModeName() const;
	int GetScreenMode() const;
	// Requires a string: "fullscreen",
	// "window" or "borderlesswindow"
	int GetScreenModeFromName(std::string mode) const;
	// Requires a string: "fullscreen",
	// "window" or "borderlesswindow"
	void SetScreenMode(std::string mode);
	sf::Vector2i GetScreenResolution() const;
	void SetScreenResolution(sf::Vector2i res);
	bool GetScreenVsync() const;
	void SetScreenVsync(bool Vsync);
	
	/////////////////////////////////////////////////////////

	bool GetShowFPS() const;
	void SetShowFPS(bool showFpsIn);
	bool GetShowGridCoords() const;
	void SetShowGridCoords(bool showGridCoordsIn);

	/////////////////////////////////////////////////////////

private:
	const std::map<std::string, int> screenModeTable = { {"borderlesswindow", 0}, {"window", 4}, {"fullscreen", 8} };
	// Screen Settings
	std::string screenMode;
	sf::Vector2i screenResolution;
	bool screenVsync;
	
	// Interface/Debug settings
	bool showFps;
	bool showGridCoords;
};