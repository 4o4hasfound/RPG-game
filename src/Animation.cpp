#include "Animation.hpp"

std::unordered_map<std::string, sf::Texture*> Animation::s_textures = {};

Animation::Animation(const std::string& spritesheet, int size, float frameDuration, AnimationDirection direction)
	: m_start(1), m_progress(0.0f), m_frameDuration(frameDuration), m_frameSize(size), m_direction(direction) {
	if (!s_textures.count(spritesheet)) {
		m_spriteSheet = new sf::Texture();
		bool success = m_spriteSheet->loadFromFile(spritesheet);
		if (!success) {
			std::cout << "error loading sprite sheet: " << spritesheet << "\n";
			return;
		}
		m_spriteSheet->setSmooth(false);
		s_textures[spritesheet] = m_spriteSheet;
	}
	else {
		m_spriteSheet = s_textures.at(spritesheet);
	}
	m_frameCount = (direction == TopDown ? m_spriteSheet->getSize().y : m_spriteSheet->getSize().x) / size;
	m_end = m_frameCount;
}

const sf::IntRect Animation::getIntRect() const {
	return sf::IntRect(
		m_direction == TopDown ? 0: static_cast<int>(m_progress / m_frameDuration) * m_frameSize,
		m_direction == TopDown ? static_cast<int>(m_progress / m_frameDuration) * m_frameSize: 0,
		m_direction == TopDown ? m_spriteSheet->getSize().x: m_frameSize,
		m_direction == TopDown ? m_frameSize : m_spriteSheet->getSize().y
	);
}

const sf::Texture* Animation::getTexture() const {
	return m_spriteSheet;
}

Frame Animation::getFrame() const {
	return {getTexture(), getIntRect()};
}

uint32_t Animation::getFrameIndex() const {
	return m_progress / m_frameDuration;
}

void Animation::setStartFrame(std::uint32_t start) {
	if (start > m_frameCount) {
		return;
	}
	m_start = start;
	if (m_progress < m_start * m_frameDuration) {
		m_progress = m_start * m_frameDuration;
	}
}

void Animation::setEndFrame(std::uint32_t end) {
	if (end > m_frameCount) {
		return;
	}
	m_end = end;
}

bool Animation::update(float dt) {
	m_progress += dt;
	if (m_progress >= m_frameDuration * m_end) {
		m_progress = m_frameDuration * (m_end) - 0.01;
		return 1;
	}
	return 0;
}

void Animation::reset() {
	m_progress = (m_start  - 1) * m_frameDuration;
}

AnimationManager::AnimationManager()
	: m_playbackSpeed(1.0f), m_currentAnimation(None) {

}

void AnimationManager::addAnimation(AnimationType type, const Animation& animation, std::function<void()> callback, std::function<void(float, float)> updateCallback) {
	m_animations[type] = animation;
	m_callbacks[type] = callback;
	m_updateCallbacks[type] = updateCallback;
	m_currentAnimation = type;
	m_keyFrameCallback[type].resize(animation.getEndFrame());
}

void AnimationManager::addKeyFrameCallback(AnimationType type, uint32_t frame, std::function<void()> callback) {
	m_keyFrameCallback[type][frame-1] = callback;
}

void AnimationManager::setCurrentAnimation(AnimationType type) {
	Animation& animation = m_animations.at(m_currentAnimation);
	animation.reset();
	m_currentAnimation = type;
}

void AnimationManager::setUpdateFunction(std::function<AnimationType()> update) {
	m_update = update;
}

AnimationType AnimationManager::getCurrentAnimation() const {
	return m_currentAnimation;
}

Animation& AnimationManager::getAnimation(AnimationType type) {
	return m_animations.at(type);
}

Frame AnimationManager::getFrame() const {
	return m_animations.at(m_currentAnimation).getFrame();
}

bool AnimationManager::update(float dt) {
	Animation& animation = m_animations.at(m_currentAnimation);

	if (m_keyFrameCallback[m_currentAnimation][animation.getFrameIndex()]) {
		m_keyFrameCallback[m_currentAnimation][animation.getFrameIndex()]();
	}

	bool end = animation.update(dt * m_playbackSpeed);
	const float progress = animation.getProgress() - (animation.getStartFrame() * animation.getFrameDuration());
	const float total = (animation.getEndFrame() - animation.getStartFrame()) * animation.getFrameDuration();

	if (m_updateCallbacks[m_currentAnimation] != nullptr) {
		m_updateCallbacks[m_currentAnimation](progress, total);
	}

	if (end) {
		if (m_callbacks[m_currentAnimation] != nullptr) {
			m_callbacks[m_currentAnimation]();
		}
	}

	AnimationType newAnimation = None;
	if (m_update != nullptr) {
		newAnimation = m_update();
	}

	if (newAnimation != None) {
		if (newAnimation != m_currentAnimation) {
			animation.reset();
		}
		else if (end) {
			animation.reset();
		}
		m_currentAnimation = newAnimation;
	}
	return end;
}

void AnimationManager::resetCurrent() {
	m_animations[m_currentAnimation].reset();
}