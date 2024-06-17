using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml.Linq;

namespace LevelEditor
{   public class LevelXml
    {
        private XElement m_xmlDocument;

        public void Open(string fileName)
        {
            m_xmlDocument = XElement.Load(fileName);
        }
    }
}
