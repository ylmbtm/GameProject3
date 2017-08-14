using UnityEngine;
using System.Collections;
using System.Xml;

namespace BIE
{
    public class CheckItemAmount : CheckCondition
    {
        public ECompare Compare = ECompare.EQ;
        public int      ItemID;
        public int      ItemNum;

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.Compare = os.GetEnum<ECompare>("Compare");
            this.ItemID  = os.GetInt32("ItemID");
            this.ItemNum = os.GetInt32("ItemNum");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "Compare", Compare.ToString());
            DCFG.Write(doc, os, "ItemID",  ItemID);
            DCFG.Write(doc, os, "ItemNum", ItemNum);
        }

        public override bool Check()
        {
            switch(Compare)
            {
                case ECompare.EQ:
                    return GTDataManager.Instance.GetItemCountById(ItemID) == ItemNum;
                case ECompare.GT:
                    return GTDataManager.Instance.GetItemCountById(ItemID) >  ItemNum;
                case ECompare.LT:
                    return GTDataManager.Instance.GetItemCountById(ItemID) <  ItemNum;
                case ECompare.GE:
                    return GTDataManager.Instance.GetItemCountById(ItemID) >= ItemNum;
                case ECompare.LE:
                    return GTDataManager.Instance.GetItemCountById(ItemID) <= ItemNum;
                default:
                    return true;
            }
        }
    }
}
