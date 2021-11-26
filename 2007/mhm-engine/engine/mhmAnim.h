#ifndef ANIM_INCLUDED
#define ANIM_INCLUDED


enum enAnimLooping {enAnimHold = 0, enAnimLoop, enAnimPingPong, enNull}; 

class clsKey
{
protected:
public:

	clsKey();
	~clsKey();

	clsString name;
	mmFloat pos;

};
//----------------------------------
class clsAnimRange
{
protected:
public:

	clsAnimRange();
	~clsAnimRange();

	clsString name;
	mmInt start;
	mmInt end;

};
//----------------------------------
class clsAnimKeys : public clsResource
{
protected:

	mmRes LoadAnim(clsString &file);

	tmpRefArr <clsKey> Keys;
	tmpRefArr <clsAnimRange> Ranges;

public:

	clsAnimKeys(clsResourceManager* presman, clsString name);
	virtual ~clsAnimKeys(void);
	mmRes Create(clsResourceParams* params);

	mmInt FindKey(const mmString name);
	clsKey & GetKey(mmInt i);

	mmBool FindKeys(const mmString name, mmInt & start, mmInt & end);


};
//----------------------------------
class clsEntity;
class clsAnimationRageController
{
protected:
public:

	clsAnimationRageController();
	~clsAnimationRageController();

	enAnimLooping Loop;

	//clsEntity * Entity;
	mmBool IsPlaying;

	mmFloat CurrentFrame;

	mmInt start;
	mmInt end;

	mmFloat start_t;
	mmFloat end_t;
	
	mmFloat dir;

	mmInt CurentNLoop;
	mmInt LoopCount;

	

	mmBool Update(clsEntity * Entity, mmFloat delta);

};






#endif // ANIM_INCLUDED
