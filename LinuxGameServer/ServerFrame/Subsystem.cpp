#include "Subsystem.h"

Subsystem::Subsystem()
{
}


Subsystem::~Subsystem()
{
}


void Subsystem::reinitialize(Application& app)
{
	uninitialize();
	initialize(app);
}


void Subsystem::defineOptions(OptionManager& options)
{
}
