/****************************************************************************
 * EZ Viewer
 * Copyright (C) 2012 huangezhao. CHINA.
 * Contact: huangezhao (huangezhao@gmail.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 ***************************************************************************/

#include "commonsetting.h"
#include "ui_commonsetting.h"

#include "config.h"

#include <QCheckBox>
#include <QColorDialog>


CommonSetting::CommonSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CommonSetting)
{
    ui->setupUi(this);
    ui->ui_groupBox->hide();

    showDialogCheckBox = ui->showDialogCheckBox;
    scaleModeCombo = ui->scaleModeCombo;
    antialiasModeCombo = ui->antialiasModeCombo;
    timerSpinBox = ui->timerSpinBox;
    colorButton = ui->colorButton;
    colorEdit = ui->colorEdit;
    colorCheckBox = ui->colorCheckBox;
    colorLabel = ui->colorLabel;
    autoRotateCheckBox = ui->autoRotateCheckBox;
    preReadingCheckBox = ui->preReadingCheckBox;
    cacheValueLabel = ui->cacheValueLabel;
    cacheValueSlider = ui->cacheValueSlider;

    alignButtonGroup = new QButtonGroup(this);
    alignButtonGroup->addButton(ui->leftTopButton, Config::AlignLeftTop);
    alignButtonGroup->addButton(ui->centerTopButton, Config::AlignCenterTop);
    alignButtonGroup->addButton(ui->rightTopButton, Config::AlignRightTop);
    alignButtonGroup->addButton(ui->leftCenterButton, Config::AlignLeftCenter);
    alignButtonGroup->addButton(ui->centerCenterButton, Config::AlignCenterCenter);
    alignButtonGroup->addButton(ui->rightCenterButton, Config::AlignRightCenter);
    alignButtonGroup->addButton(ui->leftBottomButton, Config::AlignLeftBottom);
    alignButtonGroup->addButton(ui->centerBottomButton, Config::AlignCenterBottom);
    alignButtonGroup->addButton(ui->rightBottomButton, Config::AlignRightBottom);

    initUIvalue();

    connect(showDialogCheckBox, SIGNAL(stateChanged(int)),
            SLOT(showDialogChange(int)));
    connect(scaleModeCombo, SIGNAL(currentIndexChanged(int)),
            SLOT(scaleModeChange(int)));
    connect(alignButtonGroup, SIGNAL(buttonClicked(int)),
            SLOT(alignModeChange(int)));
    connect(antialiasModeCombo, SIGNAL(currentIndexChanged(int)),
            SLOT(antialiasModeChange(int)));
    connect(colorButton, SIGNAL(clicked()), SLOT(setColor()));
    connect(colorCheckBox, SIGNAL(stateChanged(int)),
            SLOT(bgColorEnable(int)));
    connect(timerSpinBox, SIGNAL(valueChanged(int)),
            SLOT(timerIntervalChange(int)));
    connect(autoRotateCheckBox, SIGNAL(stateChanged(int)),
            SLOT(autoRotateImageChanged(int)));
    connect(preReadingCheckBox, SIGNAL(stateChanged(int)),
            SLOT(preReadingChanged(int)));
    connect(cacheValueSlider, SIGNAL(valueChanged(int)),
            SLOT(cacheValueChanged(int)));

    QDialogButtonBox *buttonBox = ui->buttonBox;
    QPushButton *button = buttonBox->addButton(QDialogButtonBox::Close);
    button->setDefault(true);
    connect(button, SIGNAL(clicked()), SIGNAL(clickClose()));
    button = buttonBox->addButton(QDialogButtonBox::RestoreDefaults);
    connect(button, SIGNAL(clicked()), SLOT(restoreDefaults()));

    Config::insertConfigWatcher(this, SLOT(initUIvalue()));
}

CommonSetting::~CommonSetting()
{
    Config::cancelConfigWatcher(this);
    delete ui;
}

void CommonSetting::initUIvalue()
{
    showDialogCheckBox->setChecked(Config::showDialog());
    scaleModeCombo->setCurrentIndex(Config::scaleMode());
    alignButtonGroup->button(Config::alignMode())->setChecked(true);
    antialiasModeCombo->setCurrentIndex(Config::antialiasMode());
    timerSpinBox->setValue(Config::timerInterval());
    autoRotateCheckBox->setChecked(Config::autoRotateImage());
    preReadingCheckBox->setChecked(Config::enablePreReading());
    cacheValueLabel->setText(QString::number(Config::cacheNum()));
    cacheValueSlider->setValue(Config::cacheNum());

    changeColorSettingUI(Config::bgColor());
    bool enabledBgColor = Config::enableBgColor();
    colorCheckBox->setChecked(enabledBgColor);
    colorLabel->setEnabled(enabledBgColor);
    colorButton->setEnabled(enabledBgColor);
    colorEdit->setEnabled(enabledBgColor);
}

void CommonSetting::changeColorSettingUI(const QColor &color)
{
    QPixmap pix(25, 25);
    pix.fill(color);
    colorButton->setIcon(QIcon(pix));
    colorEdit->setText(color.name());
}

void CommonSetting::showDialogChange(int state)
{
    Config::setShowDialog(state == Qt::Checked);
}

void CommonSetting::scaleModeChange(int index)
{
    if(index == -1) return;
    Config::setScaleMode(Config::ScaleMode(index));
}

void CommonSetting::alignModeChange(int id)
{
    if (id < Config::AlignModeBegin || id > Config::AlignModeEnd) return;
    Config::setAlignMode(Config::AlignMode(id));
}

void CommonSetting::antialiasModeChange(int index)
{
    if(index == -1) return;
    Config::setAntialiasMode(Config::AntialiasMode(index));
}

void CommonSetting::bgColorEnable(int state)
{
    bool enableBgColor = (state == Qt::Checked);
    Config::setEnableBgColor(enableBgColor);

    colorLabel->setEnabled(enableBgColor);
    colorButton->setEnabled(enableBgColor);
    colorEdit->setEnabled(enableBgColor);
}

void CommonSetting::setColor()
{
    QColor color = QColorDialog::getColor(Config::bgColor(), this);
    if (color.isValid()) {
        changeColorSettingUI(color);
        Config::setBgColor(color);
    }
}

void CommonSetting::timerIntervalChange(int val)
{
    Config::setTimerInterval(val);
}

void CommonSetting::autoRotateImageChanged(int state)
{
    Config::setAutoRotateImage(state == Qt::Checked);
}

void CommonSetting::preReadingChanged(int state)
{
    Config::setEnablePreReading(state == Qt::Checked);
}

void CommonSetting::cacheValueChanged(int val)
{
    Config::setCacheValue(val);
    cacheValueLabel->setText(QString::number(val));
}

void CommonSetting::restoreDefaults()
{
    Config::restoreDefaultsConfig();
}

