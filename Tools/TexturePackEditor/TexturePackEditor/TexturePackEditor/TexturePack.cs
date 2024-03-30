using System;
using System.Collections;
using System.Xml;
using System.IO;
using System.Drawing.Text;
using System.Security.Policy;

namespace TexturePackEditor
{
    internal class TexturePack
    {
        public TexturePack() 
        {
            FileName = "";
            m_texturePack = new ArrayList();
        }

        ~TexturePack() { }

        public bool AddTexture(string texture)
        {
            if (m_texturePack.Contains(texture))
            {
                return false;
            }

            m_texturePack.Add(texture);

            return true;
        }

        public bool RemoveTexture(string texture)
        {
            if (!m_texturePack.Contains(texture))
            {
                return false;
            }

            m_texturePack.Remove(texture);

            return true;
        }

        public bool OpenTexturePack(string filePath)
        {
            m_texturePack.Clear();

            using var reader = new StreamReader(filePath, true);

            XmlDocument xmlDoc = new XmlDocument();
            xmlDoc.Load(reader);
            FromXml(xmlDoc);

            FileName = filePath;

            return true;
        }
        public bool SaveTexturePack(string filePath)
        {
            XmlDocument xmlDoc = new XmlDocument();

            ToXml(ref xmlDoc);
            
            xmlDoc.Save(filePath);

            FileName = filePath;

            return true;
        }

        ///Format of file is
        ///<TexturePack>
        ///  <Texture file_name="" />
        ///</TexturePack>
        private void ToXml(ref XmlDocument document)
        {
            var packElement = document.CreateElement(TexturePackElementName);
            document.AppendChild(packElement);

            foreach(var filePath in m_texturePack)
            {
                var textureElement = document.CreateElement(TexturelementName);
                var filePathAttribute = document.CreateAttribute(FilePathElementName);
                filePathAttribute.Value = filePath.ToString();
                textureElement.Attributes.Append(filePathAttribute);

                packElement.AppendChild(textureElement);
            }
        }
        private void FromXml(XmlDocument document)
        {
            foreach (System.Xml.XmlElement texturePackElement in document.GetElementsByTagName(TexturePackElementName))
            {
                foreach (System.Xml.XmlElement textureElement in texturePackElement.GetElementsByTagName(TexturelementName))
                {
                    var filePathAttribute = textureElement.GetAttribute(FilePathElementName);
                    if (filePathAttribute != "")
                    {
                        m_texturePack.Add(filePathAttribute);
                    }
                }
            }
        }

        public ArrayList TexturePackContent { get { return m_texturePack; } }

        public String FileName { get; set; }
        private ArrayList m_texturePack;

        private const string TexturePackElementName = "TexturePack";
        private const string TexturelementName = "Texture";
        private const string FilePathElementName = "file_path";
    }
}
