using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace LevelEditor
{
    internal class VectorBySize<T>
    {
        public VectorBySize(uint nrElement)
        {
            m_elements = new T[nrElement];
        }

        public T this[uint index]
        {
            get
            {
                return m_elements[index];
            }
            set
            {
                m_elements[index] = value;
            }
        }

        private T[] m_elements;
    }
}
