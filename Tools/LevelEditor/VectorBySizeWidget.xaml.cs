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
    /// <summary>
    /// Interaction logic for VectorBySizeWidget.xaml
    /// </summary>
    public partial class VectorBySizeWidget : UserControl
    {
        public VectorBySizeWidget(uint nrElement)
        {
            InitializeComponent();

            //Grab the grid
            m_grid = (Grid)FindName("VectorGrid");
            m_grid.RowDefinitions.Add(new RowDefinition());
            for (uint i = 0; i < nrElement; i++)
            {
                m_grid.ColumnDefinitions.Add(new ColumnDefinition());
            }

            m_vector = new VectorBySize<float>(nrElement);
            CreateGridElements(nrElement);
        }

        private void CreateGridElements(uint nrElement)
        {
            for (uint i = 0; i < nrElement; i++)
            {
                m_labels[i] = new Label();
                m_labels[i].Content = "Element " + i;
                m_grid.Children.Add(m_labels[i]);
                Grid.SetRow(m_labels[i], (int)i);
                Grid.SetColumn(m_labels[i], 0);

                m_textBoxes[i] = new TextBox();
                m_textBoxes[i].Text = m_vector[i].ToString();
                m_grid.Children.Add(m_textBoxes[i]);
                Grid.SetRow(m_textBoxes[i], (int)i);
                Grid.SetColumn(m_textBoxes[i], 1);
            }
        }

        private VectorBySize<float> m_vector;
        private TextBox[] m_textBoxes;
        private Label[] m_labels;
        private Grid m_grid;
    }
}
