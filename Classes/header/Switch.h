#ifndef __SWITCH_H__
#define __SWITCH_H__

class Switch {
public:
    Switch() { _switch = false; }
    void setSwitch(bool sw) { _switch = sw; }
    bool isSet() { return _switch; }

private:
    bool _switch;
};

#endif  // !__SWITCH_H__