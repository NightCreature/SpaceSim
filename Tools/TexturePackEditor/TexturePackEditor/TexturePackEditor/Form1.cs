using System.Windows.Forms.Design;
using System.Collections;

namespace TexturePackEditor
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();

            //texturePackListView.View = View.Details;
            texturePackListView.LabelEdit = true;
            // texturePackListView the user to rearrange columns.
            texturePackListView.AllowColumnReorder = true;
            // Select the item and subitems when selection is made.
            texturePackListView.FullRowSelect = true;
            // Display grid lines.
            texturePackListView.GridLines = true;
            // Sort the items in the list in ascending order.
            texturePackListView.Sorting = SortOrder.Ascending;

            m_texturePack = new TexturePack();
        }

        private void button1_Click(object sender, EventArgs e)
        {

            //Browse for file
            FileDialog openFile = new System.Windows.Forms.OpenFileDialog();
            openFile.Filter = "Image Files(*.bmp;*.jpg;...)|*.BMP;*.bmp;*.JPG;*.jpg;*.png;*.PNG;*.dds;*.DDS|All files (*.*)|*.*";
            openFile.InitialDirectory = @"F:\SDK\Demo\SpaceSim\bin\Textures\";
            openFile.ShowDialog();

            if (openFile.FileName != "")
            {
                if (m_texturePack.AddTexture(openFile.FileName))
                {
                    texturePackListView.Items.Add(new ListViewItem(openFile.FileName));
                }
            }
        }

        private void button2_Click(object sender, EventArgs e)
        {
            if (textureFilePath.Text != "")
            {
                //Add Texture to pack
                if (m_texturePack.AddTexture(textureFilePath.Text))
                {
                    texturePackListView.Items.Add(new ListViewItem(textureFilePath.Text));
                }
                textureFilePath.Text = "";
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            //Remove Texture from pack
            foreach (ListViewItem item in texturePackListView.SelectedItems)
            {
                if (m_texturePack.RemoveTexture(item.Text))
                {
                    texturePackListView.Items.RemoveAt(item.Index);
                }
            }
        }

        private void openToolStripMenuItem_Click(object sender, EventArgs e)
        {
            //Open file dialog to find texture pack file
            OpenTexturePackFileDialog();
        }

        private void saveToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_texturePack.FileName == "")
            {
                SaveTexturePackFileDialog();
            }
            else
            {
                m_texturePack.SaveTexturePack(m_texturePack.FileName);
            }
        }

        private void saveAsToolStripMenuItem_Click(object sender, EventArgs e)
        {
            SaveTexturePackFileDialog();
        }

        private void closeToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void SaveTexturePackFileDialog()
        {
            //Bring up file menu and save the file
            FileDialog saveFile = new System.Windows.Forms.SaveFileDialog();
            saveFile.Filter = "*.xml|*texturepack";
            saveFile.ShowDialog();

            if (saveFile.FileName != "")
            {
                m_texturePack.SaveTexturePack(saveFile.FileName);
            }
        }

        private void OpenTexturePackFileDialog()
        {
            //Bring up file menu and save the file
            FileDialog openFile = new System.Windows.Forms.OpenFileDialog();
            openFile.Filter = "TexturePack (*.xml;*texturepack)|*.xml;*texturepack";
            openFile.ShowDialog();

            if (openFile.FileName != "")
            {
                m_texturePack.OpenTexturePack(openFile.FileName);

                if (openFile.FileName != "")
                {
                    if (m_texturePack.AddTexture(openFile.FileName))
                    {
                        texturePackListView.Items.Add(openFile.FileName);
                    }
                    textureFilePath.Text = "";

                    texturePackListView.Clear();

                    foreach (string str in m_texturePack.TexturePackContent)
                    {
                        texturePackListView.Items.Add(new ListViewItem(str));
                    }
                }
            }
        }


        private TexturePack m_texturePack;
    }
}