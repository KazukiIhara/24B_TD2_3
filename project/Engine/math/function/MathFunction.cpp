#pragma once

#include "MathFunction.h"
#include <algorithm>


float Lerp(float a, float b, float t) {
	return a + t * (b - a);
}

float Dot(const Vector3& a, const Vector3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

float DegreesToRadians(const float& degrees) {
	return degrees * (std::numbers::pi_v<float> / 180.0f);
}

float Length(const Vector3& a) {
	return std::sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
}

Vector3 Normalize(const Vector3& a) {
	float length = Length(a);
	Vector3 normalizedVector;

	if (length != 0) {
		normalizedVector.x = a.x / length;
		normalizedVector.y = a.y / length;
		normalizedVector.z = a.z / length;
	} else {
		normalizedVector.x = 0;
		normalizedVector.y = 0;
		normalizedVector.z = 0;
	}

	return normalizedVector;
}

Vector3 DegreesToRadians(const Vector3& degrees) {
	Vector3 result{};
	result.x = DegreesToRadians(degrees.x);
	result.y = DegreesToRadians(degrees.y);
	result.z = DegreesToRadians(degrees.z);
	return result;
}

Vector3 Forward(const Vector3& rotate) {
	// 基準の前方向ベクトル (ワールド座標系の Z 軸正方向)
	Vector3 baseForward = { 0.0f, 0.0f, 1.0f };

	// XYZ軸の回転行列を作成
	Matrix4x4 rotationMatrix = MakeRotateXYZMatrix(rotate);

	// 回転行列で基準ベクトルを変換
	Vector3 forward;
	forward.x = rotationMatrix.m[0][0] * baseForward.x +
		rotationMatrix.m[1][0] * baseForward.y +
		rotationMatrix.m[2][0] * baseForward.z;

	forward.y = rotationMatrix.m[0][1] * baseForward.x +
		rotationMatrix.m[1][1] * baseForward.y +
		rotationMatrix.m[2][1] * baseForward.z;

	forward.z = rotationMatrix.m[0][2] * baseForward.x +
		rotationMatrix.m[1][2] * baseForward.y +
		rotationMatrix.m[2][2] * baseForward.z;

	// 単位ベクトル化
	return Normalize(forward);
}

Vector3 ExtractionWorldPos(const Matrix4x4& m) {
	Vector3 worldPos{};
	worldPos.x = m.m[3][0];
	worldPos.y = m.m[3][1];
	worldPos.z = m.m[3][2];
	return worldPos;
}

Vector3 CatmullRomSpline(const std::vector<Vector3>& controlPoints, float t) {
	int numPoints = static_cast<int>(controlPoints.size());
	if (numPoints < 4) {
		// 4つ未満のポイントではCatmull-Romスプラインが機能しないため、直接返す
		return controlPoints[0];
	}

	// ttの計算
	float tt = t * (numPoints - 1);
	int p1 = static_cast<int>(tt);
	p1 = std::clamp(p1, 1, numPoints - 3); // p1を範囲内に制限
	int p0 = p1 - 1;
	int p2 = p1 + 1;
	int p3 = p2 + 1;

	tt = tt - p1; // 局所的なtを取得

	// Catmull-Romスプラインの計算式に基づく補間
	float tt2 = tt * tt;
	float tt3 = tt2 * tt;

	Vector3 a = controlPoints[p0] * (-0.5f) + controlPoints[p1] * (1.5f) - controlPoints[p2] * (1.5f) + controlPoints[p3] * (0.5f);
	Vector3 b = controlPoints[p0] - controlPoints[p1] * (2.5f) + controlPoints[p2] * (2.0f) - controlPoints[p3] * (0.5f);
	Vector3 c = controlPoints[p2] * 0.5f - controlPoints[p0] * 0.5f;
	Vector3 d = controlPoints[p1];

	return a * tt3 + b * tt2 + c * tt + d;
}


Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) {
	return Vector3(
		v1.x + t * (v2.x - v1.x),
		v1.y + t * (v2.y - v1.y),
		v1.z + t * (v2.z - v1.z)
	);
}

Vector3 Cross(const Vector3& a, const Vector3& b) {
	return {
		a.y * b.z - a.z * b.y,
		a.z * b.x - a.x * b.z,
		a.x * b.y - a.y * b.x
	};
}

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result{};
	Vector4 coord = { vector.x, vector.y, vector.z, 1.0f };
	Vector4 temp{};
	temp.x = coord.x * matrix.m[0][0] + coord.y * matrix.m[1][0] + coord.z * matrix.m[2][0] + coord.w * matrix.m[3][0];
	temp.y = coord.x * matrix.m[0][1] + coord.y * matrix.m[1][1] + coord.z * matrix.m[2][1] + coord.w * matrix.m[3][1];
	temp.z = coord.x * matrix.m[0][2] + coord.y * matrix.m[1][2] + coord.z * matrix.m[2][2] + coord.w * matrix.m[3][2];
	temp.w = coord.x * matrix.m[0][3] + coord.y * matrix.m[1][3] + coord.z * matrix.m[2][3] + coord.w * matrix.m[3][3];

	result.x = temp.x / temp.w;
	result.y = temp.y / temp.w;
	result.z = temp.z / temp.w;

	return result;
}

Matrix4x4 MakeIdentityMatrix4x4() {
	Matrix4x4 result{};
	result.m[0][0] = 1.0f; result.m[0][1] = 0.0f; result.m[0][2] = 0.0f; result.m[0][3] = 0.0f;
	result.m[1][0] = 0.0f; result.m[1][1] = 1.0f; result.m[1][2] = 0.0f; result.m[1][3] = 0.0f;
	result.m[2][0] = 0.0f; result.m[2][1] = 0.0f; result.m[2][2] = 1.0f; result.m[2][3] = 0.0f;
	result.m[3][0] = 0.0f; result.m[3][1] = 0.0f; result.m[3][2] = 0.0f; result.m[3][3] = 1.0f;
	return result;
}

Matrix4x4 Inverse(const Matrix4x4& a) {
	Matrix4x4 A = a;
	Matrix4x4 B = MakeIdentityMatrix4x4();

	int i, j, k;
	for (i = 0; i < 4; ++i) {
		if (A.m[i][i] == 0) {
			// ゼロ除算を避けるための行のスワップなどの処理を実装
			// 簡易的な実装ではエラー処理を挿入する必要があります。
			return MakeIdentityMatrix4x4(); // 仮のエラー処理
		}
		float scale = 1.0f / A.m[i][i];
		for (j = 0; j < 4; ++j) {
			A.m[i][j] *= scale;
			B.m[i][j] *= scale;
		}
		for (k = 0; k < 4; ++k) {
			if (k != i) {
				float factor = A.m[k][i];
				for (j = 0; j < 4; ++j) {
					A.m[k][j] -= factor * A.m[i][j];
					B.m[k][j] -= factor * B.m[i][j];
				}
			}
		}
	}

	return B;
}

Matrix4x4 Transpose(const Matrix4x4& a) {
	Matrix4x4 result;
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.m[i][j] = a.m[j][i];
		}
	}
	return result;
}

Matrix4x4 MakeInverseTransposeMatrix(const Matrix4x4& a) {
	Matrix4x4 result = Inverse(a);
	result = Transpose(result);
	return result;
}

Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result = {
		 scale.x, 0.0f, 0.0f, 0.0f,
		 0.0f, scale.y, 0.0f, 0.0f,
		 0.0f, 0.0f, scale.z, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f,
	};
	return result;
}

Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 rotateX{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, std::cos(radian), std::sin(radian), 0.0f,
		0.0f, -std::sin(radian), std::cos(radian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return rotateX;
}

Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 rotateY{
		std::cos(radian), 0.0f, -std::sin(radian), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		std::sin(radian), 0.0f, std::cos(radian), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return rotateY;
}

Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 rotateZ{
		std::cos(radian), std::sin(radian), 0.0f, 0.0f,
		-std::sin(radian), std::cos(radian), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f,
	};
	return rotateZ;
}

Matrix4x4 MakeRotateXYZMatrix(const Vector3& rotate) {
	Matrix4x4 rotateX = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateY = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZ = MakeRotateZMatrix(rotate.z);

	Matrix4x4 result = rotateX * rotateY * rotateZ;
	return result;
}

Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		translate.x, translate.y, translate.z, 1.0f
	};
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4 result = MakeScaleMatrix(scale) * MakeRotateXYZMatrix(rotate) * MakeTranslateMatrix(translate);
	return result;
}

Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate) {
	Matrix4x4 result = MakeScaleMatrix(scale) * QuaternionToMatrix4x4(rotate) * MakeTranslateMatrix(translate);
	return result;
}

Matrix4x4 MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth) {
	Matrix4x4 result = {
		width / 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, -height / 2.0f, 0.0f, 0.0f, 0.0f, 0.0f, maxDepth - minDepth, 0.0f, left + width / 2.0f, top + height / 2.0f, minDepth, 1.0f,
	};
	return result;
}

Matrix4x4 MakePerspectiveFovMatrix(float fovY, float aspectRaito, float nearClip, float farClip) {
	Matrix4x4 result =
	{
		(1.0f / aspectRaito) * (1.0f / std::tan(fovY / 2.0f)), 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f / std::tan(fovY / 2.0f), 0.0f, 0.0f,
		0.0f, 0.0f, farClip / (farClip - nearClip), 1.0f,
		0.0f, 0.0f, (-nearClip * farClip) / (farClip - nearClip), 0.0f,
	};
	return result;
}

Matrix4x4 MakeOrthographicMatrix(float left, float top, float right, float bottom, float nearClip, float farClip) {
	Matrix4x4 result =
	{
		2.0f / (right - left), 0.0f, 0.0f, 0.0f,
		0.0f, 2.0f / (top - bottom), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f / (farClip - nearClip), 0.0f,
		(left + right) / (left - right), (top + bottom) / (bottom - top), nearClip / (nearClip - farClip), 1.0f,
	};
	return result;
}

Matrix4x4 MakeUVMatrix(const Vector2& scale, const float& rotateZ, const Vector2& translate) {
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotateZ);
	Matrix4x4 scaleMatrix = MakeScaleMatrix(Vector3(scale.x, scale.y, 1.0f));
	Matrix4x4 translateMatrix = MakeTranslateMatrix(Vector3(translate.x, translate.y, 0.0f));
	return scaleMatrix * rotateZMatrix * translateMatrix;
}

Matrix4x4 QuaternionToMatrix4x4(const Quaternion& q) {
	Matrix4x4 mat;

	// クォータニオンの各成分の積を計算
	float xx = q.x * q.x;
	float yy = q.y * q.y;
	float zz = q.z * q.z;
	float xy = q.x * q.y;
	float xz = q.x * q.z;
	float yz = q.y * q.z;
	float wx = q.w * q.x;
	float wy = q.w * q.y;
	float wz = q.w * q.z;

	// 左手座標系用の回転行列を設定
	mat.m[0][0] = 1.0f - 2.0f * (yy + zz);
	mat.m[0][1] = 2.0f * (xy + wz);
	mat.m[0][2] = 2.0f * (xz - wy);
	mat.m[0][3] = 0.0f;

	mat.m[1][0] = 2.0f * (xy - wz);
	mat.m[1][1] = 1.0f - 2.0f * (xx + zz);
	mat.m[1][2] = 2.0f * (yz + wx);
	mat.m[1][3] = 0.0f;

	mat.m[2][0] = 2.0f * (xz + wy);
	mat.m[2][1] = 2.0f * (yz - wx);
	mat.m[2][2] = 1.0f - 2.0f * (xx + yy);
	mat.m[2][3] = 0.0f;

	mat.m[3][0] = 0.0f;
	mat.m[3][1] = 0.0f;
	mat.m[3][2] = 0.0f;
	mat.m[3][3] = 1.0f;

	return mat;
}

void DecomposeUVMatrix(const Matrix4x4& matrix, Vector3& scale, float& rotateZ, Vector2& translate) {
	// 平行移動成分を抽出
	translate.x = matrix.m[3][0];
	translate.y = matrix.m[3][1];

	// 回転成分を抽出
	rotateZ = std::atan2(matrix.m[1][0], matrix.m[0][0]);

	// スケール成分を抽出
	scale.x = matrix.m[0][0] / std::cos(rotateZ);
	scale.y = matrix.m[1][1] / std::cos(rotateZ);
	scale.z = 1.0f; // UV マトリックスは 2D なので z スケールは 1.0 と仮定
}

Matrix4x4 ComposeUVMatrix(const Vector3& scale, float rotateZ, const Vector2& translate) {
	Matrix4x4 matrix;

	float cosZ = std::cos(rotateZ);
	float sinZ = std::sin(rotateZ);

	// スケールと回転を適用
	matrix.m[0][0] = scale.x * cosZ;
	matrix.m[0][1] = scale.x * -sinZ;
	matrix.m[1][0] = scale.y * sinZ;
	matrix.m[1][1] = scale.y * cosZ;

	// 平行移動を適用
	matrix.m[3][0] = translate.x;
	matrix.m[3][1] = translate.y;

	// 他の要素を単位行列の値に設定
	matrix.m[0][2] = matrix.m[0][3] = 0.0f;
	matrix.m[1][2] = matrix.m[1][3] = 0.0f;
	matrix.m[2][0] = matrix.m[2][1] = matrix.m[2][2] = matrix.m[2][3] = 0.0f;
	matrix.m[3][2] = 0.0f;
	matrix.m[3][3] = 1.0f;

	return matrix;
}

Matrix4x4 MakeStretchScaleMatrix(const Vector3& baseScale, const Vector3& velocityDir, float stretchFactor) {
	// 通常スケール行列
	Matrix4x4 scaleMatrix = MakeScaleMatrix(baseScale);

	// ストレッチ方向にスケールを追加
	scaleMatrix.m[0][0] += velocityDir.x * stretchFactor;
	scaleMatrix.m[1][1] += velocityDir.y * stretchFactor;
	scaleMatrix.m[2][2] += velocityDir.z * stretchFactor;

	return scaleMatrix;
}

Quaternion Normalize(const Quaternion& q) {
	// クォータニオンの長さを計算
	float length = std::sqrt(q.x * q.x + q.y * q.y + q.z * q.z + q.w * q.w);

	// 長さがゼロの場合はエラー回避
	if (length == 0.0f) {
		// ゼロのクォータニオンを返すか、適切な処理を行う
		return Quaternion{ 0.0f, 0.0f, 0.0f, 0.0f };
	}

	// 長さで各成分を割って正規化
	return Quaternion{ q.x / length, q.y / length, q.z / length, q.w / length };
}

Quaternion Slerp(Quaternion q1, Quaternion q2, float t) {
	// クォータニオンの内積を計算
	float dot = q1.x * q2.x + q1.y * q2.y + q1.z * q2.z + q1.w * q2.w;

	// ドット積が負の場合、逆の方向に補間するために q2 を反転
	if (dot < 0.0f) {
		q2.x = -q2.x;
		q2.y = -q2.y;
		q2.z = -q2.z;
		q2.w = -q2.w;
		dot = -dot;
	}

	// 補間係数を使った係数の計算
	const float threshold = 0.9995f;
	if (dot > threshold) {
		// ドット積が閾値を超えた場合、線形補間を実行（角度が小さいため）
		Quaternion result = {
			q1.x + t * (q2.x - q1.x),
			q1.y + t * (q2.y - q1.y),
			q1.z + t * (q2.z - q1.z),
			q1.w + t * (q2.w - q1.w)
		};
		return Normalize(result); // 結果を正規化
	}

	// 角度の計算
	float theta_0 = std::acos(dot);        // θ0 = q1 と q2 間の角度
	float theta = theta_0 * t;             // θ = t に対応する角度

	// 係数の計算
	float sin_theta = std::sin(theta);
	float sin_theta_0 = std::sin(theta_0);

	float s1 = std::cos(theta) - dot * sin_theta / sin_theta_0;
	float s2 = sin_theta / sin_theta_0;

	// 補間結果の計算
	Quaternion result = {
		s1 * q1.x + s2 * q2.x,
		s1 * q1.y + s2 * q2.y,
		s1 * q1.z + s2 * q2.z,
		s1 * q1.w + s2 * q2.w
	};
	return result;
}

Quaternion QuaternionFromAxisAngle(const Vector3& axis, float angle) {
	float halfAngle = angle * 0.5f;
	float s = std::sinf(halfAngle);
	return { axis.x * s, axis.y * s, axis.z * s, std::cosf(halfAngle) };
}

Quaternion QuaternionLookRotation(const Vector3& forward, const Vector3& up) {
	Vector3 z = Normalize(forward);
	Vector3 x = Normalize(Cross(up, z));
	Vector3 y = Cross(z, x);

	Quaternion q;
	q.w = std::sqrtf(1.0f + x.x + y.y + z.z) * 0.5f;
	float w4 = 4.0f * q.w;
	q.x = (y.z - z.y) / w4;
	q.y = (z.x - x.z) / w4;
	q.z = (x.y - y.x) / w4;
	return q;
}

Vector3 QuaternionToEulerAngles(const Quaternion& q) {
	Vector3 angles;

	// Roll (x-axis rotation)
	float sinr_cosp = 2 * (q.w * q.x + q.y * q.z);
	float cosr_cosp = 1 - 2 * (q.x * q.x + q.y * q.y);
	angles.x = std::atan2(sinr_cosp, cosr_cosp);

	// Pitch (y-axis rotation)
	float sinp = 2 * (q.w * q.y - q.z * q.x);
	if (std::abs(sinp) >= 1)
		angles.y = std::copysign(std::numbers::pi_v<float> / 2, sinp); // 90度に制限
	else
		angles.y = std::asin(sinp);

	// Yaw (z-axis rotation)
	float siny_cosp = 2 * (q.w * q.z + q.x * q.y);
	float cosy_cosp = 1 - 2 * (q.y * q.y + q.z * q.z);
	angles.z = std::atan2(siny_cosp, cosy_cosp);

	return angles;
}