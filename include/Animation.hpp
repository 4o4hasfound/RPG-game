#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <iostream>
#include <SFML/Graphics.hpp>

enum AnimationType {
	None,
	Idle,
	Walk,
	Skill,
	SkillFX,
	Attack,
	Attack1,
	Attack2,
	Attack3,
	Hit,
	Death
};

struct Frame {
	const sf::Texture* texture;
	sf::IntRect rect;
};

enum AnimationDirection {
	TopDown,
	LeftRight
};

class Animation {
public:
	Animation() = default;
	Animation(const std::string& spritesheet, int size, float frameDuration, AnimationDirection direction = TopDown);

	const sf::IntRect getIntRect() const;
	const sf::Texture* getTexture() const;
	Frame getFrame() const;

	uint32_t getFrameIndex() const;

	void setStartFrame(std::uint32_t start);
	void setEndFrame(std::uint32_t end);

	int getStartFrame() const { return m_start; }
	int getEndFrame() const { return m_end; }
	int getProgress() const { return m_progress; }
	int getFrameDuration() const { return m_frameDuration; }

	// Return 1 if reached the end
	bool update(float dt);
	void reset();
private:
	static std::unordered_map<std::string, sf::Texture*> s_textures;
	sf::Texture* m_spriteSheet;

	// Duration for EACH FRAME
	float m_frameDuration;
	// Progress in milli second
	float m_progress;

	int m_frameSize;
	int m_frameCount;
	AnimationDirection m_direction;

	int m_start;
	int m_end;
};

class AnimationManager {
public:
	AnimationManager();

	Frame getFrame() const;
	void addAnimation(AnimationType type, const Animation& animation, std::function<void()> callback = nullptr, std::function<void(float, float)> updateCallback = nullptr);
	void addKeyFrameCallback(AnimationType type, uint32_t frame, std::function<void()> callback);

	void setCurrentAnimation(AnimationType type);
	void setUpdateFunction(std::function<AnimationType()> update);

	AnimationType getCurrentAnimation() const;

	Animation& getAnimation(AnimationType type);

	void setSpeed(float speed) { m_playbackSpeed = speed; }
	float getSpeed() const { return m_playbackSpeed; }

	bool update(float dt);

	void resetCurrent();
private:
	std::unordered_map<AnimationType, Animation> m_animations;
	std::unordered_map<AnimationType, std::function<void()>> m_callbacks;
	std::unordered_map<AnimationType, std::function<void(float progress, float total)>> m_updateCallbacks;
	std::unordered_map<AnimationType, std::vector< std::function<void()>>> m_keyFrameCallback;

	std::function<AnimationType()> m_update;

	AnimationType m_currentAnimation;
	float m_playbackSpeed;
};