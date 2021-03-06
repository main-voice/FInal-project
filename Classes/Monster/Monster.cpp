#include "Monster.h"
#include "GlobalPara/GlobalParameter.h"

Sprite* Monster::getSprite()
{
	return m_pMonsterSprite;
}

Monster* Monster::createMonster(cocos2d::Point position)
{
	return NULL;
}

MoveBy* Monster::createRandAction()
{
	int randomDirection = cocos2d::random(1, 4);
	static Vec2 offset(0, 0);//avoid create and release the offset frequently
	switch (randomDirection) {

		case 1: {
			offset.x = -5;
			offset.y = 0;
			setMonsterFacing(QS::kLeft);
			break;
		}
		case 2: {
			offset.x = 5;
			offset.y = 0;
			setMonsterFacing(QS::kRight);
			break;
		}
		case 3: {
			offset.x = 0;
			offset.y = -5;
			setMonsterFacing(QS::kUp);
			break;
		}
		case 4: {
			offset.x = 0;
			offset.y = 5;
			setMonsterFacing(QS::kDown);
			break;
		}
	}
	auto finalPos = m_sprite->getPosition() + offset;
	if (finalPos.x < 0.f || finalPos.y < 0.f || finalPos.x>32.f * 100.f || finalPos.y>32.f * 100.f) {
		MoveBy* moveBy = MoveBy::create(1.5f, Vec2(0, 0));
		moveBy->retain();
		return moveBy;
	}
	else {
		//log("action!");
		MoveBy* moveBy = MoveBy::create(2.5f, Vec2(offset.x, offset.y));
		moveBy->retain();
		return moveBy;
	}

}

void Monster::backIntoMap(int nowFacing)
{
	static Vec2 offset(0, 0);//avoid create and release the offset frequently
	switch (nowFacing) 
	{
		case QS::kRight: 
		{
			offset.x = -200;
			offset.y = 0;
			setMonsterFacing(QS::kLeft);
			break;
		}
		case QS::kLeft: 
		{
			offset.x = 200;
			offset.y = 0;
			setMonsterFacing(QS::kRight);
			break;
		}
		case QS::kDown: 
		{
			offset.x = 0;
			offset.y = -200;
			setMonsterFacing(QS::kUp);
			break;
		}
		case QS::kUp: 
		{
			offset.x = 0;
			offset.y = 200;
			setMonsterFacing(QS::kDown);
			break;
		}
	}
	auto finalPos = m_sprite->getPosition() + offset;
	
		//log("action!");
		MoveBy* moveBy = MoveBy::create(2.5f, Vec2(offset.x, offset.y));
		
		m_sprite->runAction(moveBy);
	
}

bool Monster::init()
{
	if (!Role::init()) 
	{
		return false;
	}
	m_pBackground = Sprite::create("Item/UI/Background-UI.png");
	m_pBackground->setScale(0.58f);
	this->addChild(m_pBackground, 2);
	//Vec2 Size= m_pMonsterSprite->getContentSize();
	m_presentHP = this->getHealth();
	m_pHealth = ProgressTimer::create(Sprite::create("Item/UI/HP-UI.png"));
	m_pHealth->setScale(0.58f);
	//m_pHealth->setPosition(m_pMonsterSprite->getPosition());
	m_pHealth->setType(ProgressTimer::Type::BAR);
	m_pHealth->setMidpoint(Point(0, 0.5));
	m_pHealth->setBarChangeRate(Point(1, 0));
	m_pHealth->setPercentage(static_cast<float>(this->getHealth())
		/ static_cast<float>(this->getMaxHealth()) * 100);
	this->addChild(m_pHealth, 2);
	setMaxHealth(10);
	scheduleUpdate();
	
	return true;
}


void Monster::resetUI()
{
	const Vec2 Size = m_pMonsterSprite->getContentSize();
	m_pHealth->setPosition(m_pMonsterSprite->getPosition() + Vec2(0, 7));
	m_pHealth->setScale(Size.x / 355 * 1.5);
	if (this->getHealth() != m_presentHP)
	{
		m_presentHP = this->getHealth();
		m_pHealth->setPercentage(static_cast<float>(this->getHealth())
			/ static_cast<float>(this->getMaxHealth()) * 100);
	}
	m_pBackground->setPosition(m_pMonsterSprite->getPosition() + Vec2(0, 7));
	m_pBackground->setScale(Size.x / 355 * 1.5);

}
void Monster::judgeMonsterDie()
{
	if (getHealth() <= 0)
	{
		auto changeColor = TintTo::create(0.3, Color3B::GRAY);
		auto dying = FadeOut::create(3);

		m_pMonsterSprite->runAction(Sequence::create(changeColor,dying ,nullptr));
		globalHero->setScore(globalHero->getScore() + 1);
		this->removeFromParent();
	}
}