#include "SettingsManager.h"

SettingsManager::SettingsManager()
{
	// Get screen settings
	sf::VideoMode mode = sf::VideoMode::getDesktopMode();

	// Screen Settings
	screenMode = "window";
	screenResolution = sf::Vector2i((int)((float)mode.width / 1.5f), (int)((float)mode.height / 1.5f));
	screenVsync = true;
	
	// Interface/Debug settings
	showFps = true;
	showGridCoords = false;
}

std::string SettingsManager::GetScreenModeName() const
{
	return screenMode;
}

int SettingsManager::GetScreenMode() const
{
	return screenModeTable.at(screenMode);
}

int SettingsManager::GetScreenModeFromName(std::string mode) const
{
	assert(mode == "fullscreen" || mode == "window" || mode == "borderlesswindow");
	return screenModeTable.at(mode);
}

void SettingsManager::SetScreenMode(std::string mode)
{
	assert(mode == "fullscreen" || mode == "window" || mode == "borderlesswindow");
	screenMode = mode;
}

sf::Vector2i SettingsManager::GetScreenResolution() const
{
	return screenResolution;
}

void SettingsManager::SetScreenResolution(sf::Vector2i res)
{
	screenResolution = res;
}

bool SettingsManager::GetScreenVsync() const
{
	return screenVsync;
}

void SettingsManager::SetScreenVsync(bool Vsync)
{
	screenVsync = Vsync;
}

bool SettingsManager::GetShowFPS() const
{
	return showFps;
}

void SettingsManager::SetShowFPS(bool showFpsIn)
{
	showFps = showFpsIn;
}

bool SettingsManager::GetShowGridCoords() const
{
	return showGridCoords;
}

void SettingsManager::SetShowGridCoords(bool showGridCoordsIn)
{
	showGridCoords = showGridCoordsIn;
}
