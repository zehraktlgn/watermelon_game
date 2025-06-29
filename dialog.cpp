#include "dialog.h"
#include "ui_dialog.h"
#include <QTextStream>
#include <QMessageBox>

QTimer *timer1;
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    // Başka bir yerde, örneğin Dialog kurucu fonksiyonunda:
    QTimer *bombaTimer = new QTimer(this);
    connect(bombaTimer, &QTimer::timeout, this, &Dialog::BombaKarpuzOlustur);
    bombaTimer->start(5000); // Her 10 saniyede bir BombaKarpuzOlustur fonksiyonunu çağır

    timer1 = new QTimer(this);
    timer1->start(500);
    connect(timer1, SIGNAL(timeout()), this, SLOT(KarpuzOlustur()));
    connect(timer1, SIGNAL(timeout()), this, SLOT(SureAzalt()));

    // Karpuz konumlarını dosyadan oku
    if (!OkuKarpuzKonumlari(":/text/konumlar.txt")) {
        QMessageBox::critical(this, "Hata", "Karpuz konumları okunamadı!");
        this->close();
    }
    // Karpuzların aşağı doğru hareketini sağlayacak QTimer'ı başlat
    karpuzTimer = new QTimer(this);
    connect(karpuzTimer, SIGNAL(timeout()), this, SLOT(KarpuzHareketEttir()));
    karpuzTimer->start(100); // Her 100 milisaniyede bir KarpuzHareketEttir fonksiyonunu çağır

}

Dialog::~Dialog()
{
    delete ui;
}

int sure=30;

void Dialog::SureAzalt()
{  ui->label_sure->setText("SÜRE: <font color='blue'>" + QString::number(sure) + "</font>");
    if (sure == 0) {
        // Zamanlayıcıyı durdur
        timer1->stop();
        karpuzTimer->stop();

        // Dosyadan mevcut maksimum skoru oku
        QFile inputFile("skor123.txt");
        if (inputFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&inputFile);
            int dosyaMaxSkor = 0; // Dosyadaki en büyük skoru saklamak için bir değişken oluştur
            while (!in.atEnd()) {
                QString line = in.readLine();
                int skor = line.toInt();
                if (skor > dosyaMaxSkor) {
                    dosyaMaxSkor = skor;
                }
            }
            maxSkor = dosyaMaxSkor; // Dosyadaki en büyük skoru maxSkor'a ata
            inputFile.close();
        } else {
            QMessageBox::critical(this, "Hata", "Skor dosyası okunamadı!");
        }

        // Oyun istatistiklerini içeren bir mesaj kutusu göster
        QString messageTitle;
        QString messageText;
        if (tıklananKarpuzSayisi > maxSkor) {
            messageTitle = "Bilgi";
            messageText ="Oyun Bitti, Tebrikler max skoru geçtiniz!\nKesilen Karpuz Sayısı: " + QString::number(tıklananKarpuzSayisi) +
                          "\nKaçırılan Karpuz Sayısı: " + QString::number(kacirilanKarpuzSayisi) +
                          "\nMaksimum Skor: " + QString::number(maxSkor);
        } else {
            messageTitle = "Bilgi";
            messageText =  "Oyun Bitti, Maalesef max skoru geçemediniz! \nKesilen Karpuz Sayısı: " + QString::number(tıklananKarpuzSayisi) +
                          "\nKaçırılan Karpuz Sayısı: " + QString::number(kacirilanKarpuzSayisi) +
                          "\nMaksimum Skor: " + QString::number(maxSkor);
        }
        QMessageBox::information(this, messageTitle, messageText);

        // Maksimum skoru dosyaya ekle
        QFile outputFile("skor123.txt");
        if (outputFile.open(QIODevice::Append | QIODevice::Text)) { // Append modunda açıyoruz
            QTextStream out(&outputFile);
            out << tıklananKarpuzSayisi << "\n"; // tıklananKarpuzSayisi dosyaya ekleniyor
            outputFile.close();
        } else {
            QMessageBox::critical(this, "Hata", "Skor dosyası yazılamadı!");
        }

        // Diyaloğu kapat
        this->accept();
    }
    sure--;
}

void Dialog::KarpuzOlustur()
{

    if (karpuzKonumlari.isEmpty()) {
        timer1->stop(); // Eğer karpuz konumları bitti ise timer'ı durdur.
        return;
    }

    // İlk karpuzu oluştur
    QPair<int, int> konum = karpuzKonumlari.takeFirst();
    QPushButton *karpuz = new QPushButton(this);
    QIcon icon(":/resimler/images/1.png");
    karpuz->setIcon(icon);
    karpuz->setIconSize(QSize(70,70));
    karpuz->setFixedSize(70, 70); // Düğme boyutunu ayarla
    karpuz->setFlat(true); // Kenarlık olmayan düğme
    karpuz->setStyleSheet("QPushButton { border: none; }"); // Kenarlığı olmayan düğme
    karpuz->setGeometry(konum.first, konum.second, 70, 70);
    karpuz->show();

    // Oluşturulan karpuzu listeye ekle
    karpuzButonlari.append(karpuz);

    // Karpuz butonuna tıklanma sinyalini bağla
    connect(karpuz, &QPushButton::clicked, this, &Dialog::KarpuzTiklandi);


}

void Dialog::KarpuzTiklandi()
{
    //kesilen karpuz sayısını hesaplamıyor
    /* QPushButton *senderButton = qobject_cast<QPushButton*>(sender());
    if (senderButton && karpuzButonlari.contains(senderButton)) {
        QIcon *icon = new QIcon();
        icon->addPixmap(QPixmap(":/resimler/images/2.png"), QIcon::Normal, QIcon::Off);
        senderButton->setIcon(*icon);
        senderButton->setIconSize(QSize(70, 70));

        QTimer::singleShot(2000, [senderButton, this](){
            karpuzButonlari.removeOne(senderButton);
            delete senderButton;
        });
    }*/
//çalışan durmuyor
   /* QPushButton *senderButton = qobject_cast<QPushButton*>(sender());
    if (senderButton && karpuzButonlari.contains(senderButton)) {
        QIcon *icon = new QIcon();
        icon->addPixmap(QPixmap(":/resimler/images/2.png"), QIcon::Normal, QIcon::Off);
        senderButton->setIcon(*icon);
        senderButton->setIconSize(QSize(70, 70));

        tıklananKarpuzSayisi++; // Tıklanan karpuz sayısını artır
        ui->label_kesilen->setText("KESİLEN KARPUZ: <font color='green'>" + QString::number(tıklananKarpuzSayisi) + "</font>");


        // Karpuz tıklandığında tıklandı özelliğini işaretle
        senderButton->setProperty("tiklandi", true);}*/

       /* QTimer::singleShot(2000, [senderButton, this](){
             //Eğer karpuz hala listede ise ve tıklanmadıysa kaçırıldı olarak kabul edilir
            if (karpuzButonlari.contains(senderButton) && !senderButton->property("tiklandi").toBool()) {
                kacirilanKarpuzSayisi++; // Kaçırılan karpuz sayısını artır
                ui->label_kacirilan->setText("KAÇIRILAN KARPUZ: " + QString::number(kacirilanKarpuzSayisi));
            }
            karpuzButonlari.removeOne(senderButton);
            delete senderButton;
        });*/


//en sonnn
    QPushButton *senderButton = qobject_cast<QPushButton*>(sender());
    if (senderButton && karpuzButonlari.contains(senderButton)) {
        QIcon *icon = new QIcon();
        icon->addPixmap(QPixmap(":/resimler/images/2.png"), QIcon::Normal, QIcon::Off);
        senderButton->setIcon(*icon);
        senderButton->setIconSize(QSize(70, 70));

        tıklananKarpuzSayisi++; // Tıklanan karpuz sayısını artır
        ui->label_kesilen->setText("KESİLEN KARPUZ: <font color='green'>" + QString::number(tıklananKarpuzSayisi) + "</font>");

        // Karpuz tıklandığında tıklandı özelliğini işaretle
        senderButton->setProperty("tiklandi", true);

        // Karpuz butonunu 3 saniye sonra kaldır
        QTimer::singleShot(3000, [senderButton, this](){
            karpuzButonlari.removeOne(senderButton);
            delete senderButton;
        });
    }

}

bool Dialog::OkuKarpuzKonumlari(const QString &dosyaAdi)
{
    QFile dosya(dosyaAdi);
    if (!dosya.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&dosya);
    while (!in.atEnd()) {
        QString satir = in.readLine();
        QStringList parcalar = satir.split(" ");
        if (parcalar.size() == 2) {
            bool ok;
            int x = parcalar[0].toInt(&ok);
            if (!ok) continue;
            int y = parcalar[1].toInt(&ok);
            if (!ok) continue;
            karpuzKonumlari.append(QPair<int, int>(x, y));
        }
    }
    dosya.close();
    return true;
}
void Dialog::KarpuzHareketEttir()
{

   //HAREKETE DEVAM EDEN AMA DÜZGÜN ÇALIŞAN
   /* foreach (QPushButton *karpuz, karpuzButonlari) {
        karpuz->move(karpuz->x(), karpuz->y() + 10); // Karpuzları 10 piksel aşağı hareket ettir

        // Eğer karpuz ekranın alt kısmına ulaşırsa
        if (karpuz->y() > this->height()) {
            // Eğer karpuz tıklanmadıysa kaçırılan karpuz sayısını artır
            if (!karpuz->property("tiklandi").toBool()) {
                kacirilanKarpuzSayisi++; // Kaçırılan karpuz sayısını artır
                ui->label_kacirilan->setText("<font color='black'>KAÇIRILAN KARPUZ: </font><font color='red'>" + QString::number(kacirilanKarpuzSayisi) + "</font>");
            }
            karpuzButonlari.removeOne(karpuz);
            delete karpuz;
        }

    }*/
//TAM ÇALIŞAN
    foreach (QPushButton *karpuz, karpuzButonlari) {
        if (karpuz->property("tiklandi").toBool()) {
            // Eğer karpuz tıklanmışsa hareketini durdur
            continue;
        }

        karpuz->move(karpuz->x(), karpuz->y() + 10); // Karpuzları 10 piksel aşağı hareket ettir

        // Eğer karpuz ekranın alt kısmına ulaşırsa
        if (karpuz->y() > this->height()) {
            // Eğer karpuz tıklanmadıysa kaçırılan karpuz sayısını artır
            kacirilanKarpuzSayisi++; // Kaçırılan karpuz sayısını artır
            ui->label_kacirilan->setText("<font color='black'>KAÇIRILAN KARPUZ: </font><font color='red'>" + QString::number(kacirilanKarpuzSayisi) + "</font>");
            karpuzButonlari.removeOne(karpuz);
            delete karpuz;
        }
    }


}
void Dialog::BombaKarpuzOlustur()
{
    // Bomba karpuzun rastgele bir konumda oluşturulması
    int x = rand() % (this->width() - 70);
    int y = rand() % (this->height() - 70);

    // Bomba karpuzun oluşturulması ve özelliklerinin ayarlanması
    QPushButton *bombaKarpuz = new QPushButton(this);
    QIcon icon(":/resimler/images/3.png");
    bombaKarpuz->setIcon(icon);
    bombaKarpuz->setIconSize(QSize(70,70));
    bombaKarpuz->setFixedSize(70, 70);
    bombaKarpuz->setFlat(true);
    bombaKarpuz->setStyleSheet("QPushButton { border: none; }");
    bombaKarpuz->setGeometry(x, y, 70, 70);
    bombaKarpuz->setProperty("bomba", true); // Bomba karpuz özelliğini ayarla
    bombaKarpuz->show();

    // Oluşturulan bomba karpuzunu listeye ekle
    karpuzButonlari.append(bombaKarpuz);

    // Bomba karpuz butonuna tıklanma sinyalini bağla
    connect(bombaKarpuz, &QPushButton::clicked, this, &Dialog::BombaKarpuzTiklandi);
}

void Dialog::BombaKarpuzTiklandi()
{
    // Oyunu sonlandır
    OyunuSonlandir();
}

void Dialog::OyunuSonlandir()
{
    // Oyun istatistiklerini içeren bir mesaj kutusu göster
    QMessageBox::information(this, "Oyun Bitti", "Bomba karpuzuna dokundunuz! Oyun sona erdi.");

    // Diyaloğu kapat
    this->accept();
}

