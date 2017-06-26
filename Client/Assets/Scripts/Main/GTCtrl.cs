using UnityEngine;
using System.Collections;
using System.Collections.Generic;

public class GTCtrl : GTSingleton<GTCtrl>
{
    private List<ICtrl> m_ctrls = new List<ICtrl>();

    public void AddLoginCtrl()
    {
        AddCtrl<LoginCtrl>(false);
    }

    public void AddAllCtrls()
    {
        AddCtrl<AdventureCtrl>(true);
        AddCtrl<AwardCtrl>(true);
        AddCtrl<BagCtrl>(true);
        AddCtrl<EquipCtrl>(true);
        AddCtrl<GemCtrl>(true);
        AddCtrl<MountCtrl>(true);
        AddCtrl<PartnerCtrl>(true);
        AddCtrl<PetCtrl>(true);
        AddCtrl<RelicsCtrl>(true);
        AddCtrl<RaidCtrl>(true);
        AddCtrl<RoleCtrl>(true);
        AddCtrl<SkillCtrl>(true);
        AddCtrl<StoreCtrl>(true);
        AddCtrl<TaskCtrl>(true);
    }

    void AddCtrl<T>(bool addToList) where T: ICtrl
    {
        T ctrl = System.Activator.CreateInstance<T>();
        ctrl.AddListener();
        if (addToList)
        {
            m_ctrls.Add(ctrl);
        }
    }

    void DelCtrls()
    {
        for (int i = 0; i < m_ctrls.Count; i++)
        {
            m_ctrls[i].DelListener();
        }
        m_ctrls.Clear();
    }
}
