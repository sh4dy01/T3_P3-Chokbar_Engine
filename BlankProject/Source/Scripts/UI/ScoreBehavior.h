#include <string>
#include <vector>
#include <DirectXMath.h>

#define NUM_DIGIT_PER_SCORE 6

class Digit;

class ScoreBehavior : public CustomComponent
{
public:
	ScoreBehavior();
	~ScoreBehavior();

	void Awake() override;
	void Start() override;
	void Update() override;

	float GetScore() const { return m_score; }
	void UpdateScore(UINT newScore);

	void CreateDigit(UINT index);

private:
	UINT m_score;
	std::array<Digit*, NUM_DIGIT_PER_SCORE> digitsObjects;
	bool m_isScoreDirty;

	void OnDelete();
	void UpdateScoreDisplay();
};
