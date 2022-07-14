#include "principal.h"
#include "ui_principal.h"

#define DEFAULT_ANCHO 3

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    // Instanciando la imagen (creando)
    mImagen = new QImage(this->size(),QImage::Format_ARGB32_Premultiplied);
    // Rellenar la imagen de color blanco
    mImagen->fill(Qt::white);
    // Instanciar el Painter a partir de la imagen
    mPainter = new QPainter(mImagen);
    mPainter->setRenderHint(QPainter::Antialiasing);
    // Inicializar otras variables
    mPuedeDibujar = false;
    mColor = Qt::black;
    mAncho = DEFAULT_ANCHO;
    mNumLineas = 0;
}

Principal::~Principal()
{
    delete ui;
    delete mPainter;
    delete mImagen;
}

void Principal::paintEvent(QPaintEvent *event)
{
    // Crear el painter de la ventana principal
    QPainter painter(this);
    // Dibujar la imagen
    painter.drawImage(0, 0, *mImagen);
    // Acepatr el evento
    event->accept();
}

void Principal::mousePressEvent(QMouseEvent *event)
{
    //Levanta la bandera (para que se pueda dibujar)
    mPuedeDibujar = true;
    //Captura la posicion (x, y) del mouse
    mInicial = event->pos();
    //Acepta el evento
    event->accept();
}

void Principal::mouseMoveEvent(QMouseEvent *event)
{
    // Validar si se puede dibujar
    if ( !mPuedeDibujar ) {

        //Acepta el evento
        event->accept();
        //Salir del metodo
        return;
    }
    // Capturar el punto a donde se mueve el mouse
    mFinal = event->pos();
    // Crear un pincel y establecer atributos
    if (ui->actionLibre->isChecked()==true){
        on_actionLibre_triggered();
    }


}

void Principal::mouseReleaseEvent(QMouseEvent *event)
{
    //Bajar la bandera (no se puede dibujar)
    mPuedeDibujar = false;
    // Aceptar el vento
    event->accept();
    if (ui->actionLineas->isChecked()==true){
        on_actionLineas_triggered();
    }
    if (ui->actionRect_nculos->isChecked()==true){
        on_actionRect_nculos_triggered();
    }
    if(ui->actionCircunferencias->isChecked()==true){
        on_actionCircunferencias_triggered();
    }
}


void Principal::on_actionAncho_triggered()
{
    mAncho = QInputDialog::getInt(this,
                                  "Ancho del pincel",
                                  "Ingrese el ancho del pincel de dibujo",
                                  mAncho,
                                  1, 100);
}

void Principal::on_actionSalir_triggered()
{
    this->close();
}

void Principal::on_actionColor_triggered()
{
    mColor = QColorDialog::getColor(mColor,
                                    this,
                                    "Color del pincel");
}

void Principal::on_actionNuevo_triggered()
{
    mImagen->fill(Qt::white);
    mNumLineas = 0;
    update();
}

void Principal::on_actionGuardar_triggered()
{
    //Abrir archivo de dialogo para obtener el nombre del archivo
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                         "Guardar imagen",
                                                         QString(),
                                                         "Imágenes .png (*.png)");
    //Validar que el nombre del archivo no sea vacio
    if ( !nombreArchivo.isEmpty() ){
        //Guardar imagen
        if (mImagen->save(nombreArchivo))
            //Si todo va bien, muestra un mensaje de informacion
            QMessageBox::information(this,
                                     "Guardar imagen",
                                     "Archivo almacenado en: " + nombreArchivo);
        else
            //Si hay algun error muestro advertencia
            QMessageBox::warning(this,
                                 "Guardar imagen",
                                 "No se pudo almacenar la imagen.");
    }
}



void Principal::on_actionCircunferencias_triggered()
{
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    mPainter->setPen(pincel);
    mPainter->drawEllipse(mInicial.x(),mInicial.y(),mFinal.x()-mInicial.x(),mFinal.y()-mInicial.y());
    update();
}



void Principal::on_actionLibre_checkableChanged(bool checkable)
{
    if(checkable==true){
        QPen pincel;
        pincel.setColor(mColor);
        pincel.setWidth(mAncho);
        mPainter->setPen(pincel);
        mPainter->drawEllipse(QRect(QPoint(mInicial.x(),mFinal.y()),QSize(5,5)));
       // Crear un pincel y establecer atributos
       // Mostrar el número de líneas en la barra de estado
       ui->statusbar->showMessage("Número de líneas: " + QString::number(++mNumLineas));
       // Interfaz actualizada
       update();
       //Actualizacion punto inicial
       mInicial = mFinal;
    }
}



void Principal::on_actionLibre_triggered()
{
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    // Dibujar una linea
    mPainter->setPen(pincel);
    mPainter->drawLine(mInicial, mFinal);
    // Mostrar el número de líneas en la barra de estado
    ui->statusbar->showMessage("Número de líneas: " + QString::number(++mNumLineas));
    // Actualizar la interfaz
    update();
    // actualizar el punto inicial
    mInicial = mFinal;
}

void Principal::on_actionLineas_triggered()
{
        QPen pincel;
        pincel.setColor(mColor);
        pincel.setWidth(mAncho);
        // Dibujar una linea
        mPainter->setPen(pincel);
        QLine line(mInicial.x(),mInicial.y(),mFinal.x(),mFinal.y());
        mPainter->drawLine(line);
        update();
}


void Principal::on_actionRect_nculos_triggered()
{
    QPen pincel;
    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    mPainter->setPen(pincel);
    if(mInicial != mFinal)
    {
        mAncho = 10;
        mPainter->drawRect(mInicial.x(),mInicial.y(),mFinal.x()-mInicial.x(),mFinal.y()-mInicial.y());
        update();
    }
}
