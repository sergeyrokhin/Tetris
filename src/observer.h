#pragma once
class Observer
{
	bool update_requaired = true;
public:
	void SetUpdateRequired(bool on = true) { update_requaired = on; }

};

