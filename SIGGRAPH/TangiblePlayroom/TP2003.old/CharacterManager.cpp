#include "CharacterManager.h"

// �L�����N�^�̏Փ˔���̗L���̐؂�ւ�	
bool CharacterManager::SetCharCollisionable(int id, bool b)
{
	switch(id){

	case PENGUIN1:
		objects[objects.penBase]->SetCollisionable(b);
		break;
	case PENGUIN2:
		objects[objects.oppPenBase]->SetCollisionable(b);
		break;
	case PENGUIN3:
		objects[objects.oppPenBase+1]->SetCollisionable(b);
		break;
	case PUCK1:
		objects[objects.puckBase]->SetCollisionable(b);
		break;
	case PUCK2:
		objects[objects.puckBase+1]->SetCollisionable(b);
		break;
	case PUCK3:
		objects[objects.puckBase+2]->SetCollisionable(b);
		break;
	case PUCK4:
		objects[objects.puckBase+3]->SetCollisionable(b);
		break;
	default:
		return false;
	}

	return true;
}
	
// �L�����N�^�̈ʒu�ݒ�
bool CharacterManager::SetCharPosition(int id, Affinef af)
{ 
	return true;
}

// �L�����N�^�̕\����\���̐؂�ւ�
bool CharacterManager::SetCharVisible(int id, bool b)
{

	switch(id){
	case PENGUIN1: m_game->penguin[0].root->SetVisible(b); break;
	case PENGUIN2: m_game->penguin[1].root->SetVisible(b); break;
	case PENGUIN3: m_game->penguin[2].root->SetVisible(b); break;
	case PUCK1: m_game->puck[0].root->SetVisible(b); break;
	case PUCK2: m_game->puck[1].root->SetVisible(b); break;
	case PUCK3: m_game->puck[2].root->SetVisible(b); break;
	case PUCK4: m_game->puck[3].root->SetVisible(b); break;
	default: break;
	}
	return true;
}

void CharacterManager::SetGame(Game *game)
{
	m_game = game;
}
