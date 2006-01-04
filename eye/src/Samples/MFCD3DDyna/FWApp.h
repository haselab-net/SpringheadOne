#ifndef FWAPP_H
#define FWAPP_H
namespace Spr{;

class FWApp{
public:
	UTRef<SGScene> scene;
	UTRef<GRRender> render; 
	void Init();
	void Load(std::string fn);
	void Save(std::string fn);
	void Draw();
	void Step();
};

}
#endif
