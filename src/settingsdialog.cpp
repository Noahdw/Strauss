#include "settingsdialog.h"

SettingsDialog::SettingsDialog(AudioEngine *audioEngine, QWidget *parent)
    :audio_engine(audioEngine), QDialog(parent)
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
    auto audio_text1 = new QLabel(tr("Output buffer size:"));
    block_size_label = new QLabel("Output Latency: " + QString::number(getLatency(g_blocksize),'g',1));
    audio_layout->addWidget(audio_text1);
    audio_layout->addWidget(block_size_slider);
    audio_layout->addWidget(block_size_label);

    connect(block_size_slider, &QSlider::valueChanged,this,&SettingsDialog::blockSizeSliderChanged);
    block_size_slider->setRange(64,8192);
    block_size_slider->setOrientation(Qt::Horizontal);
    block_size_slider->setValue(g_blocksize);

    // MIDI VIEW
    stacked_widget->addWidget(midi_frame);
    auto midi_layout = new QVBoxLayout;
    midi_layout->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    midi_layout->setSpacing(0);
    midi_layout->setContentsMargins(0,0,0,0);
    midi_frame->setLayout(midi_layout);
    auto midi_text1 = new QLabel(tr("Plugin folders"));
    plugin_list_widget = new QListWidget;
    plugin_group_box = new QGroupBox;
    QVBoxLayout *plugin_vbox = new QVBoxLayout;
    QPushButton *plugin_add_button = new QPushButton("Add");
    QPushButton *plugin_remove_button = new QPushButton("Remove");
    plugin_vbox->addWidget(plugin_add_button);
    plugin_vbox->addWidget(plugin_remove_button);
    plugin_group_box->setLayout(plugin_vbox);
    midi_layout->addWidget(midi_text1);
    midi_layout->addWidget(plugin_list_widget);
    midi_layout->addWidget(plugin_group_box);
    connect(plugin_add_button,&QPushButton::clicked,this,&SettingsDialog::addPluginFolder);
    connect(plugin_remove_button,&QPushButton::clicked,this,&SettingsDialog::removePluginFolder);
    QSettings settings;
    settings.beginGroup("folders");
    int fcount = settings.beginReadArray("paths");
    for (int i = 0; i < fcount; ++i)
    {
        settings.setArrayIndex(i);
        plugin_list_widget->addItem(settings.value("path").toString());
    }
    settings.endArray();
    settings.endGroup();
}

void SettingsDialog::accept()
{
    if (g_blocksize != block_size_slider->value())
    {
        audio_engine->stopPortAudio();
        audio_engine->changeBlockSize(g_blocksize,block_size_slider->value());
        audio_engine->startPortAudio();
        audio_engine->openStream();
        audio_engine->startStream();
    }
    QSettings settings;
    settings.beginGroup("folders");
    settings.beginWriteArray("paths");
    for (int i = 0; i < plugin_list_widget->count(); ++i)
    {
        settings.setArrayIndex(i);
        settings.setValue("path",plugin_list_widget->item(i)->text());
    }
    settings.endArray();
    settings.endGroup();
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

void SettingsDialog::addPluginFolder()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Select plugin folder"),
                                                    "/home",
                                                    QFileDialog::ShowDirsOnly
                                                    | QFileDialog::DontResolveSymlinks);
    if (dir != "")
    {
        plugin_list_widget->addItem(dir);
    }
}

void SettingsDialog::removePluginFolder()
{
    QList<QListWidgetItem*> items = plugin_list_widget->selectedItems();
    foreach(QListWidgetItem* item, items){
        plugin_list_widget->removeItemWidget(item);
        delete item;
    }
}

double getLatency(int sampleSize)
{
    return sampleSize / (double)g_sampleRate * 1000;
}
