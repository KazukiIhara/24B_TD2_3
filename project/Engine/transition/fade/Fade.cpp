#include "Fade.h"

#include "framework/SUGER.h"

void Fade::Initialize() {
	// フェード処理用のオブジェクトを作成
	SUGER::Create2DObject("9999_Fade", "white.png");
	// コントローラ初期化
	controller_.Initialize("9999_Fade");
	// サイズ設定
	controller_.SetSize(Vector2(WindowManager::kClientWidth, WindowManager::kClientHeight));
	// カラー設定
	controller_.SetColor(Vector4(0.0f, 0.0f, 0.0f, 1.0f));
}

void Fade::Update() {
	// フェード状態による分岐
	switch (status_) {
		case FadeStatus::None:
			// 何もしない
			break;
		case FadeStatus::FadeIn:
			// フェードイン中の更新処理
			UpdateFadeIn();
			break;
		case FadeStatus::FadeOut:
			// フェードアウト中の更新処理
			UpdateFadeOut();
			break;
	}
}

void Fade::Start(FadeStatus status, float duration) {
	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

void Fade::UpdateFadeIn() {
	// 1フレーム分の秒数をカウントアップ
	counter_ += kDeltaTime_;
	// フェード継続時間に達したら打ち止め
	if (counter_ >= duration_) {
		counter_ = duration_;
	}
	// 0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどアルファ値を小さくする
	controller_.SetColor(Vector4(0.0f, 0.0f, 0.0f, std::clamp(1.0f - counter_ / duration_, 0.0f, 1.0f)));
}

void Fade::UpdateFadeOut() {
	// 1フレーム分の秒数をカウントアップ
	counter_ += kDeltaTime_;
	// フェード継続時間に達したら打ち止め
	if (counter_ >= duration_) {
		counter_ = duration_;
	}
	// 0.0fから1.0fの間で、経過時間がフェード継続時間に近づくほどアルファ値を小さくする
	controller_.SetColor(Vector4(0.0f, 0.0f, 0.0f, std::clamp(counter_ / duration_, 0.0f, 1.0f)));
}

void Fade::Stop() {
	status_ = FadeStatus::None;
}

bool Fade::IsFinished() const {
	// フェード状態による分岐
	switch (status_) {
		case FadeStatus::FadeIn:
		case FadeStatus::FadeOut:
			return counter_ >= duration_;
	}
	return true;
}
