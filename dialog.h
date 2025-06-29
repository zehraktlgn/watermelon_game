#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include<QTimer>
#include<QPushButton>
#include<QIcon>
#include<QPixmap>
#include<QSize>
#include<QFile>
#include<QDebug>
#include<QIODevice>
#include<QImage>
#include<QRgb>
#include<QBitmap>
#include<QDateTime>
#include<QFileDialog>
#include<QRandomGenerator>
#include<QRandomGenerator64>
QT_BEGIN_NAMESPACE
namespace Ui {
class Dialog;
}
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

    bool OkuKarpuzKonumlari(const QString &dosyaAdi);
    void KarpuzTiklandi();


    void OyunuSonlandir();
    void BombaKarpuzOlustur();
    void BombaKarpuzTiklandi();
public slots:
    void SureAzalt();

    void KarpuzOlustur();
    void KarpuzHareketEttir();
private:
    Ui::Dialog *ui;
    QList<QPair<int, int>> karpuzKonumlari;
    QList<QPushButton*> karpuzButonlari; // karpuz butonlarını tutacak liste
    QTimer *karpuzTimer;
    int tıklananKarpuzSayisi = 0;
    int kacirilanKarpuzSayisi = 0;
    int maxSkor=0;

};
#endif // DIALOG_H
