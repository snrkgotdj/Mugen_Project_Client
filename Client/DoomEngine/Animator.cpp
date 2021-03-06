#include "Animator.h"
#include "Animation.h"
#include "MeshRender.h"

#include "Device.h"

#define ANIMCLIP_MAX 32


CAnimator::CAnimator()
	: m_pCurAnimClip(NULL)
	, m_bEnable(true)
{
}

CAnimator::CAnimator(const CAnimator & _com)
	: CComponent(_com)
	, m_bEnable(true)
{
	map<wstring, CAnimation*>::const_iterator iter = _com.m_mapAnimClip.begin();
	map<wstring, CAnimation*>::const_iterator iterEnd = _com.m_mapAnimClip.end();

	for (iter; iter != iterEnd; ++iter)
	{
		CAnimation* pAnim = iter->second->Clone();
		pAnim->SetAnimator(this);

		m_mapAnimClip.insert(make_pair(iter->first, pAnim));

		if (iter == _com.m_mapAnimClip.begin())
			m_pCurAnimClip = pAnim;
	}
}

CAnimator::~CAnimator()
{
	Safe_Delete_Map(m_mapAnimClip);
}

void CAnimator::Awake()
{
	//if (!m_mapAnimClip.empty())
	//{
	//	map<wstring, CAnimation*>::iterator iter = m_mapAnimClip.begin();
	//
	//	m_pCurAnimClip = iter->second;
	//}
}

void CAnimator::Start()
{
}

void CAnimator::Update()
{
	if (NULL != m_pCurAnimClip&&m_bEnable)
		m_pCurAnimClip->Update();

	if (m_mapAnimClip.size() == 1)
		m_pCurAnimClip = m_mapAnimClip.begin()->second;
}

void CAnimator::Render()
{
}

void CAnimator::AddAnimClip(const wstring& _strKey, CAnimation * _pClip)
{
	_pClip->AddFrame(_pClip->GetVecFrame()[_pClip->GetMaxFrame()]);//더미 프레임 하나 넣기
	m_mapAnimClip.insert(make_pair(_strKey, _pClip));
	_pClip->SetAnimator(this);
}

void CAnimator::AddAnimClip(CAnimation * _pClip)
{
	_pClip->AddFrame(_pClip->GetVecFrame()[_pClip->GetMaxFrame()]);//더미 프레임 하나 넣기
	m_mapAnimClip.insert(make_pair(_pClip->GetClipName(), _pClip));
	_pClip->SetAnimator(this);
}

wstring CAnimator::GetCurAnimClipName()
{
	return m_pCurAnimClip->GetClipName();
}

void CAnimator::GetAnimCilps(vector<tAnimPair>& _pVecOut)
{
	map<wstring, CAnimation*>::iterator iter = m_mapAnimClip.begin();
	map<wstring, CAnimation*>::iterator iterEnd = m_mapAnimClip.end();

	for (iter; iter != iterEnd; ++iter)
	{
		_pVecOut.push_back(tAnimPair{ iter->first,iter->second });
	}
}

void CAnimator::SetAnim(const wstring & _strClipName)
{
	map<wstring, CAnimation*>::iterator iter = m_mapAnimClip.find(_strClipName);
	if (iter == m_mapAnimClip.end())
		return;

	m_pCurAnimClip = iter->second;
	m_pCurAnimClip->SetCurFrame(0);
}

void CAnimator::Save(FILE * _pFile)
{
	UINT iType = (UINT)COMPONENT_TYPE::ANIMATOR;
	fwrite(&iType, sizeof(UINT), 1, _pFile);

	vector<tAnimPair> vecAnim;
	GetAnimCilps(vecAnim);

	UINT iSize = vecAnim.size();
	fwrite(&iSize, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iSize; ++i)
	{
		CAnimation* pAnim = vecAnim[i].pAnim;
		bool bRepeat = pAnim->GetClipRepeat();
		fwrite(&bRepeat, sizeof(bool), 1, _pFile);
		SaveWString(pAnim->GetClipName(), _pFile);
		SaveWString(pAnim->GetNextClipName(), _pFile);
		SaveWString(pAnim->GetFileName(), _pFile);
		SaveWString(pAnim->GetTexKey(), _pFile);
	}
}

void CAnimator::Load(FILE * _pFile)
{
	UINT iSize;
	fread(&iSize, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iSize; ++i)
	{
		bool bRepeat;
		fread(&bRepeat, sizeof(bool), 1, _pFile);
		wstring strName = LoadWString(_pFile);
		wstring strNext = LoadWString(_pFile);
		wstring strFileName = LoadWString(_pFile);
		wstring strTexKey = LoadWString(_pFile);
		CAnimation* pAnim = new CAnimation(strName, bRepeat, strNext);
		pAnim->AddFrameByFile(strFileName, strTexKey);
		AddAnimClip(pAnim);
		if (i == 0)
			SetAnim(pAnim);
	}
}
