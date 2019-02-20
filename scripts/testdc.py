import time
from ctypes import *

class RiapsDc:
    def __init__(self):
        self.lib = CDLL("./libriaps.so")
        pass

    def JoinGroup(self, group_id, component_id, component_name, application_name, actor_name):
        self.group_ptr = self.lib.join_group(
            c_char_p(group_id.encode('utf-8')),
            c_char_p(component_id.encode('utf-8')),
            c_char_p(component_name.encode('utf-8')),
            c_char_p(application_name.encode('utf-8')),
            c_char_p(actor_name.encode('utf-8'))
        )

    def GetMemberCount(self):
        self.lib.get_member_count(self.group_ptr)

    def LeaveGroup(self, group_id, component_id):
        self.lib.join_group(c_char_p(group_id.encode('utf-8')), c_char_p(component_id.encode('utf-8')))

    def __del__(self):
        self.lib.free_group(self.group_ptr);

dc = RiapsDc()
dc2 = RiapsDc()
dc.JoinGroup("gr_id", "cmp_id", "comp_name", "app_name", "act_name")
dc2.JoinGroup("gr_id", "cmp_id", "comp_name2", "app_name", "act_name")
time.sleep(5)
print(dc.GetMemberCount())
print(dc2.GetMemberCount())
time.sleep(5)
print("done")