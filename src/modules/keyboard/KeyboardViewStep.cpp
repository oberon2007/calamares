/* === This file is part of Calamares - <http://github.com/calamares> ===
 *
 *   Copyright 2014, Teo Mrnjavac <teo@kde.org>
 *
 *   Calamares is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Calamares is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Calamares. If not, see <http://www.gnu.org/licenses/>.
 */

#include "KeyboardViewStep.h"

#include "JobQueue.h"
#include "GlobalStorage.h"

#include "KeyboardPage.h"


KeyboardViewStep::KeyboardViewStep( QObject* parent )
    : Calamares::ViewStep( parent )
    , m_widget( new KeyboardPage() )
    , m_nextEnabled( false )
{
    m_widget->init();
    m_nextEnabled = true;
    emit nextStatusChanged( m_nextEnabled );
}


KeyboardViewStep::~KeyboardViewStep()
{
    if ( m_widget && m_widget->parent() == nullptr )
        m_widget->deleteLater();
}


QString
KeyboardViewStep::prettyName() const
{
    return tr( "Keyboard" );
}


QString
KeyboardViewStep::prettyStatus() const
{
    return m_prettyStatus;
}


QWidget*
KeyboardViewStep::widget()
{
    return m_widget;
}


void
KeyboardViewStep::next()
{
    //TODO: actually save those settings somewhere
    emit done();
}


void
KeyboardViewStep::back()
{}


bool
KeyboardViewStep::isNextEnabled() const
{
    return m_nextEnabled;
}


bool
KeyboardViewStep::isAtBeginning() const
{
    return true;
}


bool
KeyboardViewStep::isAtEnd() const
{
    return true;
}


QList< Calamares::job_ptr >
KeyboardViewStep::jobs() const
{
    return m_jobs;
}


void
KeyboardViewStep::onLeave()
{
    m_widget->finalize();
    m_jobs = m_widget->createJobs();
    m_prettyStatus = m_widget->prettyStatus();
}


void
KeyboardViewStep::setConfigurationMap( const QVariantMap& configurationMap )
{
    // Save the settings to the global settings for the SetKeyboardLayoutJob to use
    Calamares::GlobalStorage* gs = Calamares::JobQueue::instance()->globalStorage();

    if ( configurationMap.contains( "xOrgConfFileName" ) &&
         configurationMap.value( "xOrgConfFileName" ).type() == QVariant::String &&
         !configurationMap.value( "xOrgConfFileName" ).toString().isEmpty() )
    {
        gs->insert( "keyboardXOrgConfFileName",
                    configurationMap.value( "xOrgConfFileName" ) );
    }
    else
    {
        gs->insert( "keyboardXOrgConfFileName", "00-keyboard.conf" );
    }

    if ( configurationMap.contains( "convertedKeymapPath" ) &&
         configurationMap.value( "convertedKeymapPath" ).type() == QVariant::String &&
         !configurationMap.value( "convertedKeymapPath" ).toString().isEmpty() )
    {
        gs->insert( "keyboardConvertedKeymapPath",
                    configurationMap.value( "convertedKeymapPath" ) );
    }
    else
    {
        gs->remove( "keyboardConvertedKeymapPath" );
    }
}
