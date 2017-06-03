#ifndef TRACKCOLLECTION_H
#define TRACKCOLLECTION_H

#include <QtSql>
#include <QList>
#include <QSharedPointer>
#include <QSqlDatabase>

#include "preferences/usersettings.h"
#include "library/basetrackcache.h"
#include "library/crate/cratestorage.h"
#include "library/dao/trackdao.h"
#include "library/dao/cuedao.h"
#include "library/dao/playlistdao.h"
#include "library/dao/analysisdao.h"
#include "library/dao/directorydao.h"
#include "library/dao/libraryhashdao.h"
#include "util/db/dbconnection.h"

// forward declaration(s)
class Track;

#define AUTODJ_TABLE "Auto DJ"

class BpmDetector;

// Manages everything around tracks.
class TrackCollection : public QObject {
    Q_OBJECT

  public:
    static const QString kDefaultSchemaFile;
    static const int kRequiredSchemaVersion;

    explicit TrackCollection(
            UserSettingsPointer pConfig,
            const QString& schemaFile = QString());
    ~TrackCollection() override;

    QSqlDatabase database() const {
        return m_dbConnection.database();
    }

    const CrateStorage& crates() const {
        return m_crates;
    }

    TrackDAO& getTrackDAO() {
        return m_trackDao;
    }
    PlaylistDAO& getPlaylistDAO() {
        return m_playlistDao;
    }
    DirectoryDAO& getDirectoryDAO() {
        return m_directoryDao;
    }
    AnalysisDao& getAnalysisDAO() {
        return m_analysisDao;
    }

    QSharedPointer<BaseTrackCache> getTrackSource() const {
        return m_pTrackSource;
    }
    void setTrackSource(QSharedPointer<BaseTrackCache> pTrackSource);

    void cancelLibraryScan();

    void relocateDirectory(QString oldDir, QString newDir);

    bool hideTracks(const QList<TrackId>& trackIds);
    bool unhideTracks(const QList<TrackId>& trackIds);

    bool purgeTracks(const QList<TrackId>& trackIds);
    bool purgeTracks(const QDir& dir);

    bool insertCrate(const Crate& crate, CrateId* pCrateId = nullptr);
    bool updateCrate(const Crate& crate);
    bool deleteCrate(CrateId crateId);
    bool addCrateTracks(CrateId crateId, const QList<TrackId>& trackIds);
    bool removeCrateTracks(CrateId crateId, const QList<TrackId>& trackIds);

    bool updateAutoDjCrate(CrateId crateId, bool isAutoDjSource);

  signals:
    void crateInserted(CrateId id);
    void crateUpdated(CrateId id);
    void crateDeleted(CrateId id);

    void crateTracksChanged(
            CrateId crate,
            const QList<TrackId>& tracksAdded,
            const QList<TrackId>& tracksRemoved);
    void crateSummaryChanged(
            const QSet<CrateId>& crates);

  private:
    bool upgradeDatabaseSchema(
            const QString& schemaFile,
            int schemaVersion) const;

    DbConnection m_dbConnection;

    PlaylistDAO m_playlistDao;
    CrateStorage m_crates;
    CueDAO m_cueDao;
    DirectoryDAO m_directoryDao;
    AnalysisDao m_analysisDao;
    LibraryHashDAO m_libraryHashDao;
    TrackDAO m_trackDao;

    QSharedPointer<BaseTrackCache> m_pTrackSource;
};

#endif // TRACKCOLLECTION_H
