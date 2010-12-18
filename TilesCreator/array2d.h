/*  Copyright 2009, 2010 L�o Grange

This file is part of Tiles Creator.

Tiles Creator is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Tiles Creator is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tiles Creator.  If not, see <http://www.gnu.org/licenses/>. */

/**
  * @file array2d.h
  * @brief Fichier contenant le patron de classe Array2D.
  * @author L�o Grange
  * @version 1.0
  * @date 10/12/2009
  */

#ifndef ARRAY2D_H
#define ARRAY2D_H

#include <cstring>
#include <vector>

/**
  * @brief Le patron de classe Array2D fournit un moyen de g�rer nativement des tableaux 2D.
  *
  * Array2D permet de manipuler simplement des tableaux en deux dimensions, de n'importe
  * quel type qui contient un constructeur par d�faut ainsi qu'un operator=(T).\n
  * Il est possible de construire une instance de Array2D<T> � partir d'un std::vector<T>, ou d'obtenir
  * un std::vector<T> grace � toVector().
  */
template <typename T> class Array2D {
private:
    T* m_map;
    int m_width;
    int m_height;

public:
    /**
      * @brief Constructeur par d�faut.
      *
      * Construit un tableau de rows lignes sur columns colonnes.
      * @param rows nombre de lignes.
      * @param columns nombre de colonnes.
      */
    Array2D(int rows=0, int columns=0);

    /**
      * @brief Constructeur par recopie.
      *
      * Construit un tableau � partir d'un autre tableau.
      * @param other une r�f�rence vers un autre tableau.
      * @see operator=()
      */
    Array2D(const Array2D<T> &other) {
        m_map = 0;
        clone(other);
    }

    /**
      * @brief Constructeur depuis un std::vector<T>.
      *
      * Construit un nouveau Array2D<T> � partir d'un std::vector<T>.
      * Ces std::vector<T> sont compos�s de chaque ligne du tableau 2D les unes apr�s
      * les autres :\n <CENTER>
      * <TABLE BORDER="2" CELLSPACING="0"> <CAPTION ALIGN="TOP"> std::vector<T> avec columns = 2 </CAPTION>
      * <TR> <TD>1</TD> <TD>2</TD> <TD>3</TD> <TD>4</TD> </TR> </TABLE>
      * <TABLE BORDER="2" CELLSPACING="0"> <CAPTION ALIGN="TOP">Array2D<T> correspondant</CAPTION>
      * <TR> <TD>1</TD> <TD>2</TD> </TR>  <TR> <TD>3</TD> <TD>4</TD> </TR> </TABLE>
      * </CENTER> Si un std::vector<T> a une taille non divisible par le nombre de colonnes
      * indiqu�, le nombre de lignes du tableau sera le r�sultat entier de taille/columns.
      * @param v r�f�rence du vector de base.
      * @param columns nombre de colonnes.
      * @see toVector()
      */
    Array2D(const std::vector<T> &v, int columns);

    /**
      * @brief Renvoi un std::vector<T> correspondant au tableau.
      *
      * @return un std::vector<T> qui corrspond au tableau.
      * @see Array2D(const std::vector<T>&, int)
      */
    std::vector<T> toVector() const;

    /**
      * @brief Copie un autre tableau.
      *
      * @param other une r�f�rence du tableau � copier.
      * @return une r�f�rence vers *this.
      * @see clone()
      */
    Array2D& operator= (const Array2D<T> &other) {
        clone(other);
        return (*this);
    }

    /**
      * @brief Renvoi un �l�ment du tableau.
      *
      * Renvoi l'�l�ment du tableau situ� � la ligne row de la colonne column. \n
      * Si les coordon�es d�passent du tableau, une instance de T construite avec le
      * constructeur par d�faut est renvoy�e.
      * @param row ligne de l'�l�ment.
      * @param column colonne de l'�l�ment.
      * @return l'�l�ment si celui-ci existe, T() dans le cas contraire.
      * @see operator()()
      */
    T at (int row, int column) const {
        if (row >= 0 && column >= 0 && m_width > column && m_height > row)
            return m_map[column+row*m_width];
        else return T();
    }

    /**
      * @brief Renvoi la lvalue d'un �l�ment du tableau.
      *
      * Renvoi la r�f�rence de l'�l�ment du tableau situ� � la ligne row de la colonne column.
      * @param row ligne de l'�l�ment.
      * @param column colonne de l'�l�ment.
      * @return une r�f�rence vers l'�l�ment si celui-ci existe.
      * @warning Si les coordon�es d�passent du tableau, rien n'est pr�vu et la m�thode ne
      *     retourne rien, donc l'application plante. Pensez � v�rifier que vous �tes bien
      *     dans les limites.
      * @see at()     @see columnCount()    @see rowCount()
      */
    T& operator() (int row, int column) {
        if (row >= 0 && column >= 0 && m_width > column && m_height > row)
            return m_map[column+row*m_width];
    }

    /**
      * @brief Ajoute une nouvelle ligne au tableau.
      * @param newRow contenu � ins�rer dans la nouvelle ligne.\n
      *         Si \a newRow est plus grand que le nombre de colonnes, seul les columnCount() premiers items
      *         seront utilis�s.\n
      *         Si \a newRow est plus petit que le nombre de colonnes, la ligne d�butera par les valeurs de
      *         \a newRow et finira par des items construits avec le constructeur par d�faut de T.
      * @see appendColumn(), resize(), getRow()
      */
    void appendRow(std::vector<T> newRow = std::vector<T>());

    /**
      * @brief Ajoute une nouvelle colonne au tableau.
      * @param newColumn contenu � ins�rer dans la nouvelle colonne.\n
      *         Si \a newColumn est plus grand que le nombre de lignes, seul les rowCount() premiers items
      *         seront utilis�s.\n
      *         Si \a newColumn est plus petit que le nombre de lignes, la colonne d�butera par les valeurs de
      *         \a newColumn et finira par des items construits avec le constructeur par d�faut de T.
      * @see appendRow(), resize(), getColumn()
      */
    void appendColumn(std::vector<T> newColumn = std::vector<T>());


    /**
      * @brief Renvoi le nombre de colonnes du tableau.
      * @return le nombre de colonnes du tableau.
      * @see rowCount()
      */
    int columnCount() const{
        return m_width;
    }

    /**
      * @brief Renvoi le nombre de lignes du tableau.
      * @return le nombre de lignes du tableau.
      * @see columnsCount()
      */
    int rowCount() const{
        return m_height;
    }

    /**
      * @brief Redimensionne le tableau.
      *
      * Le tableau est redimensionn� avec rows lignes et columns colonnes. Si
      * les dimensions sont plus grandes qu'au paravant, de nouveaux �l�ments
      * construits avec le constructeur par d�faut de T sont cr��s et ce qui
      * existait est conserv�. Sinon, ce qui existait est conserv� uniquement
      * dans les nouvelles dimensions et les �l�ments qui sortes de celles-ci
      * sont suprimm�s.
      * @param rows nouveau nombre de lignes.
      * @param columns nouveau nombre de colonnes.
      * @see appendRow()  @see appendColumn()   @see clear()
      */
    void resize(int rows, int columns);

    /**
      * @brief Efface le tableau et le rend de dimensions nulles.
      * @see resize()
      */
    void clear() {
        if (m_map != 0) delete[] m_map;
        m_map = 0;
        m_width = 0;
        m_height = 0;
    }

    /**
      * @brief Renvoie une ligne du tableau.
      * @return un std::vector contenant la ligne du tableau.
      * @param rowNumber num�ro de la ligne � renvoyer.
      * @see getColumn(), at()
      */
    std::vector<T> getRow(int rowNumber);

    /**
      * @brief Renvoie une colonne du tableau.
      * @return un std::vector contenant la colonne du tableau.
      * @param columnNumber num�ro de la colonne � renvoyer.
      * @see getRow(), at()
      */
    std::vector<T> getColumn(int columnNumber);

private:
    /**
      * @brief Clone un autre tableau.
      *
      * Duplique un tableau (m�me nombre de lignes, m�me nombre de colonnes,
      * contenu identique au moment de la copie).
      * @param a une r�f�rence du tableau � dupliquer.
      * @see operator=()
      */
    void clone(const Array2D<T> &a);
};


template <typename T> Array2D<T>::Array2D(int rows, int columns) {
    m_map = 0;
    m_width = columns;
    m_height = rows;
    if (m_width != 0 && m_height != 0)
        m_map = new T[m_width*m_height];
}


template <typename T> Array2D<T>::Array2D(const std::vector<T> &v, int columns) {
    if (columns > 0) {
        m_height = v.size()/columns;
        m_width = columns;
        if (m_width != 0 && m_height != 0) {
            m_map = new T[m_width*m_height];
            for (int i=0; i<m_height; i++) {
                for (int j=0; j<m_width; j++)
                    m_map[i*m_width+j] = v.at(i*m_width+j);
            }
        }
        else m_map = 0;
    }
    else {
        m_height = 0;
        m_width = 0;
        m_map = 0;
    }
}


template <typename T> std::vector<T> Array2D<T>::toVector() const{
    std::vector<T> ret;
    ret.resize(m_width*m_height);
    for (int i=0; i<m_height; i++) {
        for (int j=0; j<m_width; j++)
            ret[i*m_width+j] = m_map[i*m_width+j];
    }
    return ret;
}




template <typename T> void Array2D<T>::clone(const Array2D<T> &a) {
    if (m_map != 0) delete[] m_map;
    m_width = a.m_width;
    m_height = a.m_height;
    if (a.m_map == 0 || m_width <= 0 || m_height <= 0) m_map = 0;
    else {
        m_map = new T[m_width*m_height];
        memcpy(m_map, a.m_map, sizeof(T)*m_width*m_height);
    }
}



template <typename T> void Array2D<T>::appendRow(std::vector<T> newRow) {
    if (m_width >= 0) {
        T* tmpMap = new T[(m_height+1)*m_width];
        if (m_map != 0) {
            memcpy(tmpMap, m_map, sizeof(T)*m_width*m_height);
            delete[] m_map;
        }
        m_map = tmpMap;
    }
    else m_map = 0;
    m_height++;
    for (int i=0; i<m_width && i<newRow.size(); i++) this->operator ()(m_height-1, i) = newRow[i];
}



template <typename T> void Array2D<T>::appendColumn(std::vector<T> newColumn) {
    if (m_height >= 0) {
        T* tmpMap = new T[m_height*(m_width+1)];
        if (m_map != 0) {
            for (int i=0; i<m_height; i++)
                memcpy(tmpMap+i*(m_width+1), m_map+i*m_width, sizeof(T)*m_width);
            delete[] m_map;
        }
        m_map = tmpMap;
    }
    else m_map = 0;
    m_width++;
    for (int i=0; i<m_height && i<newColumn.size(); i++) this->operator ()(i, m_width-1) = newColumn[i];
}



template <typename T> void Array2D<T>::resize(int rows, int columns) {
    if (rows*columns != 0) {
        T *tmpMap;
        tmpMap = new T[columns*rows];
        if (m_map != 0) {
            int columnMin = m_width;
            if (m_width > columns) columnMin = columns;
            int rowMin = m_height;
            if (m_height > rows) rowMin = rows;
            for (int i=0; i<rowMin; i++)
                memcpy(tmpMap+columns*i, m_map+m_width*i, sizeof(T)*columnMin);
            delete[] m_map;
        }
        m_map = tmpMap;
    }
    else clear();
    m_width = columns;
    m_height = rows;
}


template <typename T> std::vector<T> Array2D<T>::getRow(int rowNumber) {
    std::vector<T> ret;
    if (rowNumber < m_height) {
        ret.resize(m_width);
        for (int i=0; i<m_width; i++) ret[i] = m_map[rowNumber*m_width+i];
    }
    return ret;
}


template <typename T> std::vector<T> Array2D<T>::getColumn(int columnNumber) {
    std::vector<T> ret;
    if (columnNumber < m_width) {
        ret.resize(m_height);
        for (int i=0; i<m_height; i++) ret[i] = m_map[i*m_width+columnNumber];
    }
    return ret;
}

#endif //ARRAY2D_H
