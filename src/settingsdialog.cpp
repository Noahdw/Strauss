#include "settingsdialog.h"

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    setFixedSize(600,600);
    midi_frame = new QFrame;
    audio_frame = new QFrame;
    list_widget = new QListWidget;
    stacked_widget = new QStackedWidget;
    block_size_slider = new QSlider;
    h_layout = new QHBoxLayout;
    auto main_layout = new QVBoxLayout;
    button_box = new QDialogButtonBox(QDialogButtonBox::Ok
                                      | QDialogButtonBox::Cancel);
    h_layout->addWidget(list_widget);
    h_layout->addWidget(stacked_widget);
    main_layout->addLayout(h_layout);
    main_layout->addWidget(button_box);
    setLayout(main_layout);

    QListWidgetItem *midiItem  = new QListWidgetItem("Midi");
    QListWidgetItem *audioItem = new QListWidgetItem("Audio");
    list_widget->addItem(audioItem);
    list_widget->addItem(midiItem);


    connect(button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(list_widget,&QListWidget::clicked,this, &SettingsDialog::listWidgetClicked);

    setWindowTitle(tr("Settings"));
    Qt::WindowFlags flags = Qt::WindowTitleHint | Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    //AUDIO VIEW
    stacked_widget->addWidget(audio_frame);
    auto audio_layout = new QVBoxLayout;
    audio_layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    audio_layout->setSpacing(0);
    audio_layout->setContentsMargins(0,0,0,0);
    audio_frame->setLayout(audio_layout);
    auto midi_text1 = new QLabel(tr("Input buffer size:"));
    block_size_label = new QLabel("Input Latency: " + QString::number(getLatency(g_blocksize),'g',1));
    audio_layout->addWidget(midi_text1);
    audio_layout->addWidget(block_size_slider);
    audio_layout->addWidget(block_size_label);

    connect(block_size_slider, &QSlider::valueChanged,this,&SettingsDialog::blockSizeSliderChanged);
    block_size_slider->setRange(64,131072);
    block_size_slider->setOrientation(Qt::Horizontal);

    // MIDI VIEW
    stacked_widget->addWidget(midi_frame);
    auto midi_layout = new QVBoxLayout;
    midi_layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    midi_layout->setSpacing(0);
    midi_layout->setContentsMargins(0,0,0,0);
    midi_frame->setLayout(midi_layout);
}

void SettingsDialog::accept()
{
    //Has to stop audio engine or it will crash

   done(1);
}

void SettingsDialog::reject()
{
    done(0);
}

void SettingsDialog::listWidgetClicked()
{
    QModelIndex index = list_widget->currentIndex();
    stacked_widget->setCurrentIndex(index.row());
}

void SettingsDialog::blockSizeSliderChanged(int value)
{
    block_size_label->setText("Latency: " + QString::number(getLatency(value),'f',1) + " ms");
}

double getLatency(int sampleSize)
{
    return sampleSize / (double)g_sampleRate * 1000;
}
