using UnityEngine;
using System.Collections;
using System.Xml;

namespace BIE
{
    public class CheckWeekday : CheckCondition
    {
        public int  Weekday;

        public override void Read(XmlElement os)
        {
            base.Read(os);
            this.Weekday = os.GetInt32("Weekday");
        }

        public override void Write(XmlDocument doc, XmlElement os)
        {
            base.Write(doc, os);
            DCFG.Write(doc, os, "Weekday", Weekday);
        }

        public override bool Check()
        {
            return System.DateTime.Now.DayOfWeek == (System.DayOfWeek)Weekday;
        }
    }
}

