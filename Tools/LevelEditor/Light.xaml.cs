using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace LevelEditor
{
    //<!--<Light name = "light_1" type="2" att="0.0f" lin_att="0.025f" quad_att="0.0f" inner_cone="0.21" outer_cone="0.52" exponent="0" range="1100">
    //    <Vector3 x = "525" y="-75" z="-200" />
    //    <Vector3 x = "0" y="0" z="1" />
    //    <Color r = "1" g="1" b="1" a="1"/>
    //    <Color r = "0" g="1" b="0" a="1"/>
    //</Light>-->
    class LightData
    {
        public LightData()
        {
            Name = "";
            Type = 0;
            Attenuation = new VectorBySize<float>(3);
            Cone = new VectorBySize<float>(2);
            Exponent = 0;
            Range = 0;
            Position = new VectorBySize<float>(3);
            Direction = new VectorBySize<float>(3);
            Color = new VectorBySize<float>(4);
            Specular = new VectorBySize<float>(4);
            Shininess = 0;
        }

        public String Name { get; set; }
        public uint Type { get; set; }
        public VectorBySize<float> Attenuation { get; set; }
        public VectorBySize<float> Cone { get; set; }
        public float Exponent { get; set; }
        public float Range { get; set; }
        public VectorBySize<float> Position { get; set; }
        public VectorBySize<float> Direction { get; set; }
        public VectorBySize<float> Color { get; set; }
        public VectorBySize<float> Specular { get; set; }
        public float Shininess { get; set; }
    }


    /// <summary>
    /// Interaction logic for Light.xaml
    /// </summary>
    public partial class Light : UserControl
    {
        public Light()
        {
            m_lightData = new LightData();
            InitializeComponent();

            m_grid = (Grid)FindName("LightGrid");
            
            m_grid.Children.Add(new Label { Content = "Name" });
            TextBox name = new TextBox();
            name.SetBinding(TextBox.TextProperty, new Binding("Name"));
            m_grid.Children.Add(new TextBox { Text = m_lightData.Name });

        }

        private Grid m_grid;
        private static uint nrFields = 10;
        private LightData m_lightData;
    }
}
