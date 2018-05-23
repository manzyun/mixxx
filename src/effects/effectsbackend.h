#ifndef EFFECTSBACKEND_H
#define EFFECTSBACKEND_H

#include <QObject>
#include <QList>
#include <QSet>
#include <QString>

#include "preferences/usersettings.h"
#include "effects/effect.h"
#include "effects/effectinstantiator.h"

class EffectsManager;
class EffectsBackend;
class EffectProcessor;

// An EffectsBackend is an implementation of a provider of Effect's for use
// within the rest of Mixxx. The job of the EffectsBackend is to both enumerate
// and instantiate effects.
class EffectsBackend : public QObject {
    Q_OBJECT
  public:
    EffectsBackend(UserSettingsPointer pConfig, QObject* pParent, QString name);
    virtual ~EffectsBackend();

    virtual const QString getName() const;

    // returns a list sorted like it should be displayed in the GUI 
    virtual const QList<QString> getEffectIds() const;
    virtual EffectManifestPointer getManifest(const QString& effectId) const;
    virtual bool getVisibility(const QString& effectId) const;
    virtual void addVisibleEffect(const QString& effectId);
    virtual bool canInstantiateEffect(const QString& effectId) const;
    virtual EffectPointer instantiateEffect(
            EffectsManager* pEffectsManager, const QString& effectId);

  signals:
    void effectRegistered(EffectManifestPointer);

  protected:
    void registerEffect(const QString& id,
                        EffectManifestPointer pManifest,
                        EffectInstantiatorPointer pInstantiator);

    template <typename EffectProcessorImpl>
    void registerEffect() {
        registerEffect(
                EffectProcessorImpl::getId(),
                EffectProcessorImpl::getManifest(),
                EffectInstantiatorPointer(
                        new EffectProcessorInstantiator<EffectProcessorImpl>()));
    }
    
    UserSettingsPointer m_pConfig;

  private:
    class RegisteredEffect {
      public:
        RegisteredEffect(EffectManifestPointer pManifest, EffectInstantiatorPointer pInitator)
            : m_pManifest(pManifest),
              m_pInitator(pInitator) {
        }

        RegisteredEffect() {
        }

        EffectManifestPointer manifest() const { return m_pManifest; };
        EffectInstantiatorPointer initiator() const { return m_pInitator; };
      private:
        EffectManifestPointer m_pManifest;
        EffectInstantiatorPointer m_pInitator;
    };

    QString m_name;
    QMap<QString, RegisteredEffect> m_registeredEffects;
    QList<QString> m_effectIds;
    QList<QString> m_visibleEffectIds;
};

#endif /* EFFECTSBACKEND_H */
