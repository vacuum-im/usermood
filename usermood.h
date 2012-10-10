#ifndef USERMOOD_H
#define USERMOOD_H

#include "definitions.h"
#include "iusermood.h"
#include "usermooddialog.h"
#include "ui_usermooddialog.h"

#include <interfaces/imainwindow.h>
#include <interfaces/inotifications.h>
#include <interfaces/ioptionsmanager.h>
#include <interfaces/ipepmanager.h>
#include <interfaces/ipluginmanager.h>
#include <interfaces/ipresence.h>
#include <interfaces/irostersmodel.h>
#include <interfaces/irostersview.h>
#include <interfaces/iservicediscovery.h>
#include <interfaces/ixmppstreams.h>

#include <definitions/menuicons.h>
#include <definitions/notificationdataroles.h>
#include <definitions/notificationtypeorders.h>
#include <definitions/notificationtypes.h>
#include <definitions/optionvalues.h>
#include <definitions/resources.h>
#include <definitions/rosterdataholderorders.h>
#include <definitions/rosterindextyperole.h>
#include <definitions/rosterlabelorders.h>
#include <definitions/rostertooltiporders.h>

#include <utils/action.h>
#include <utils/menu.h>
#include <utils/options.h>
#include <utils/widgetmanager.h>

class UserMood :
	public QObject,
	public IPlugin,
	public IUserMood,
	public IRosterDataHolder,
	public IPEPHandler
{
	Q_OBJECT;
	Q_INTERFACES(IPlugin IUserMood IRosterDataHolder IPEPHandler);

public:
	UserMood();
	~UserMood();
	//IPlugin
	virtual QObject *instance() { return this; }
	virtual QUuid pluginUuid() const { return USERMOOD_UUID; }
	virtual void pluginInfo(IPluginInfo *APluginInfo);
	virtual bool initConnections(IPluginManager *APluginManager, int &AInitOrder);
	virtual bool initObjects();
	virtual bool initSettings() { return true; }
	virtual bool startPlugin() { return true; }

	//IRosterDataHolder
	virtual int rosterDataOrder() const;
	virtual QList<int> rosterDataRoles() const;
	virtual QList<int> rosterDataTypes() const;
	virtual QVariant rosterData(const IRosterIndex *AIndex, int ARole) const;
	virtual bool setRosterData(IRosterIndex *AIndex, int ARole, const QVariant &AValue);

	//IPEPHandler
	virtual bool processPEPEvent(const Jid &AStreamJid, const Stanza &AStanza);

	//IUserMood
	virtual void setMood(const Jid &AStreamJid, const Mood &AMood);
	virtual QIcon moodIcon(const QString &keyname) const;
	virtual QString moodName(const QString &keyname) const;
	virtual QString contactMoodKey(const Jid &contactJid) const;
	virtual QIcon contactMoodIcon(const Jid &contactJid) const;
	virtual QString contactMoodName(const Jid &contactJid) const;
	virtual QString contactMoodText(const Jid &contactJid) const;

signals:
	//IRosterDataHolder
	void rosterDataChanged(IRosterIndex *AIndex = NULL, int ARole = 0);

protected slots:
//    void onOptionsOpened();
//    void onOptionsChanged(const OptionsNode &ANode);
//    void onRosterIndexInserted(const Jid &AContactJid, const QString &AMood);
	void onRosterIndexToolTips(IRosterIndex *AIndex, int ALabelId, QMultiMap<int, QString> &AToolTips);
	void onShowNotification(const Jid &AStreamJid, const Jid &AContactJid);
	void onNotificationActivated(int ANotifyId);
	void onNotificationRemoved(int ANotifyId);
	void onRosterIndexInserted(IRosterIndex *AIndex);
	void onRosterIndexContextMenu(const QList<IRosterIndex *> &AIndexes, int ALabelId, Menu *AMenu);
	void onSetMoodActionTriggered(bool);
	void onApplicationQuit();

protected:
	void addMood(const QString &name, const QString &locname);
	Action *createSetMoodAction(const Jid &AStreamJid, const QString &AFeature, QObject *AParent) const;
	void setContactMood(const Jid &AStreamJid, const Jid &ASenderJid, const Mood &AMood);

	//IRosterDataHolder
	void updateDataHolder(const Jid &ASenderJid = Jid::null);

private:
	IMainWindowPlugin *FMainWindowPlugin;
	IPresencePlugin *FPresencePlugin;
	IPEPManager *FPEPManager;
	IServiceDiscovery *FDiscovery;
	IXmppStreams *FXmppStreams;
	IOptionsManager *FOptionsManager;
	IRostersModel *FRostersModel;
	IRostersViewPlugin *FRostersViewPlugin;
	INotifications *FNotifications;

	int handlerId;
	int FUserMoodLabelId;

	QMap<int, Jid> FNotifies;
	QHash<QString, MoodData> FMoodsCatalog;
	QHash<Jid, MoodContact> FMoodsContacts;

};

#endif // USERMOOD_H


