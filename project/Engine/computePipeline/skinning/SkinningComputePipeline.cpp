#include "SkinningComputePipeline.h"

#include <cassert>

#include "debugTools/logger/Logger.h"
#include "directX/dxgi/DXGIManager.h"

void SkinningComputePipeline::Initialize(DXGIManager* dxgi) {
	SetDXGI(dxgi);
	InitializeDxCompiler();
	CreateRootSignature();
	CompileShaders();
	CreatePipelineStateObject();
}

ID3D12RootSignature* SkinningComputePipeline::GetRootSignature() {
	return rootSignature_.Get();
}

ID3D12PipelineState* SkinningComputePipeline::GetPipelineState() {
	return pipelineState_.Get();
}

void SkinningComputePipeline::CreateRootSignature() {
	HRESULT hr = S_FALSE;

	// SRV Descriptor Ranges (それぞれt0, t1, t2を割り当てる)
	D3D12_DESCRIPTOR_RANGE srvRanges0[1] = {};
	srvRanges0[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	srvRanges0[0].NumDescriptors = 1;              // t0
	srvRanges0[0].BaseShaderRegister = 0;          // t0から始まる
	srvRanges0[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE srvRanges1[1] = {};
	srvRanges1[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	srvRanges1[0].NumDescriptors = 1;              // t1
	srvRanges1[0].BaseShaderRegister = 1;          // t1から始まる
	srvRanges1[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE srvRanges2[1] = {};
	srvRanges2[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	srvRanges2[0].NumDescriptors = 1;              // t2
	srvRanges2[0].BaseShaderRegister = 2;          // t2から始まる
	srvRanges2[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// UAV Range
	D3D12_DESCRIPTOR_RANGE uavRanges[1] = {};
	uavRanges[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
	uavRanges[0].NumDescriptors = 1;                // u0のみ
	uavRanges[0].BaseShaderRegister = 0;            // u0から始まる
	uavRanges[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// Root Parameters
	D3D12_ROOT_PARAMETER rootParams[5] = {};

	// SRV Descriptor Table用パラメータ (t0)
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[0].DescriptorTable.NumDescriptorRanges = _countof(srvRanges0);
	rootParams[0].DescriptorTable.pDescriptorRanges = srvRanges0;
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// SRV Descriptor Table用パラメータ (t1)
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[1].DescriptorTable.NumDescriptorRanges = _countof(srvRanges1);
	rootParams[1].DescriptorTable.pDescriptorRanges = srvRanges1;
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// SRV Descriptor Table用パラメータ (t2)
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[2].DescriptorTable.NumDescriptorRanges = _countof(srvRanges2);
	rootParams[2].DescriptorTable.pDescriptorRanges = srvRanges2;
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// UAV Descriptor Table用パラメータ (u0)
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParams[3].DescriptorTable.NumDescriptorRanges = _countof(uavRanges);
	rootParams[3].DescriptorTable.pDescriptorRanges = uavRanges;
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// CBV用パラメータ (b0)
	rootParams[4].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParams[4].Descriptor.ShaderRegister = 0; // b0
	rootParams[4].Descriptor.RegisterSpace = 0;
	rootParams[4].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

	// Static Sampler
	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].RegisterSpace = 0;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;

	// Root Signature
	D3D12_ROOT_SIGNATURE_DESC rootSigDesc = {};
	rootSigDesc.NumParameters = _countof(rootParams);
	rootSigDesc.pParameters = rootParams;
	rootSigDesc.NumStaticSamplers = _countof(staticSamplers);
	rootSigDesc.pStaticSamplers = staticSamplers;
	rootSigDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_NONE;

	// シリアライズ
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	hr = D3D12SerializeRootSignature(&rootSigDesc,
		D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	if (FAILED(hr)) {
		Logger::Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	// Root Signature作成
	hr = dxgi_->GetDevice()->CreateRootSignature(0, signatureBlob->GetBufferPointer(),
		signatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(hr));

}

void SkinningComputePipeline::CompileShaders() {
	computeShaderBlob_ = nullptr;
	computeShaderBlob_ = CompileShader(L"resources/shaders/skinning/skinning.CS.hlsl",
		L"cs_6_0", dxcUtils, dxcCompiler, includeHandler);
	assert(computeShaderBlob_ != nullptr);
}

void SkinningComputePipeline::CreatePipelineStateObject() {
	HRESULT hr;

	assert(rootSignature_);
	assert(computeShaderBlob_);

	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineStateDesc{};
	computePipelineStateDesc.CS = {
		.pShaderBytecode = computeShaderBlob_->GetBufferPointer(),
		.BytecodeLength = computeShaderBlob_->GetBufferSize(),
	};
	computePipelineStateDesc.pRootSignature = rootSignature_.Get();
	pipelineState_ = nullptr;
	hr = dxgi_->GetDevice()->CreateComputePipelineState(&computePipelineStateDesc, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(hr));
}

void SkinningComputePipeline::InitializeDxCompiler() {
	HRESULT hr = S_FALSE;
	// dxCompilerを初期化
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr));
	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));
}

ComPtr<ID3DBlob> SkinningComputePipeline::CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler) {
	// これからシェーダーをコンパイルする旨をログに出す
	Logger::Log(Logger::ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
	// hlslファイルを読む
	Microsoft::WRL::ComPtr<IDxcBlobEncoding> shaderSource = nullptr;
	HRESULT hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	// 読めなかったら止める
	assert(SUCCEEDED(hr));
	// 読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer{};
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;//UTF8の文字コードであることを通知

	LPCWSTR arguments[] = {
		filePath.c_str(),// コンパイル対象のhlslファイル名
		L"-E",L"main",// エントリーポイントの指定。基本的にmain以外にはしない
		L"-T",profile,// ShaderProfileの設定
		L"-Zi",L"-Qembed_debug",// デバッグ用の情報を埋め込む
		L"-Od",// 最適化を外しておく
		L"-Zpr",// メモリレイアウトは行優先
	};
	// 実際にシェーダーをコンパイルする
	Microsoft::WRL::ComPtr<IDxcResult> shaderResult = nullptr;
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,	// 読み込んだファイル
		arguments,				// コンパイルオプション
		_countof(arguments),	// コンパイルオプションの数
		includeHandler,			// includeが含まれた諸々
		IID_PPV_ARGS(&shaderResult)// コンパイル結果
	);
	// コンパイルエラーではなく、dxcが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));

	// 警告、エラーが出てたらログに出して止める
	Microsoft::WRL::ComPtr<IDxcBlobUtf8> shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Logger::Log(shaderError->GetStringPointer());
		assert(false);
	}

	// コンパイル結果から実行用のバイナリ部分を取得
	Microsoft::WRL::ComPtr<ID3DBlob> shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	// 成功したログを出す
	Logger::Log(Logger::ConvertString(std::format(L"Compile Succeeded,path:{},profile:{}\n", filePath, profile)));

	// 実行用のバイナリを追加
	return shaderBlob;
}

void SkinningComputePipeline::SetDXGI(DXGIManager* dxgi) {
	assert(dxgi);
	dxgi_ = dxgi;
}
