/// @file
/// @brief Defines the SpeciesWidget class.

#ifndef SPECIES_WIDGET_H
#define SPECIES_WIDGET_H

#include <memory>

#include <QWidget>
#include <QMenu>

#include "fish_annotator/common/species.h"
#include "../../src/common/ui_species_widget.h"

namespace fish_annotator {

class SpeciesWidget : public QWidget {
  Q_OBJECT
public:
  /// @brief Constructor.
  ///
  /// @param species Species object.
  /// @param parent Parent widget.
  explicit SpeciesWidget(const Species &species, QWidget *parent = 0);

  /// @brief Gets const reference to species used to construct this widget.
  const Species &getSpecies() const {return species_;}

  /// @brief Sets widget values according to input Species object.
  void setSpecies(const Species &species);

  /// @brief Updates individual counts.
  void setCount(uint64_t count);

private slots:
  /// @brief Adds an individual of this species.
  void on_addIndividual_clicked();

  /// @brief Adds an individual of this species and subspecies.
  void onSubspeciesClicked();

signals:
  /// @brief Signal for individual added.
  void individualAdded(std::string species, std::string subspecies);

private:
  /// @brief Widget loaded from ui file.
  std::unique_ptr<Ui::SpeciesWidget> ui_;

  /// @brief Subspecies menu.
  std::unique_ptr<QMenu> subspecies_menu_;

  /// @brief Species used to construct this widget.
  Species species_;
};

} // namespace fish_annotator

#endif // SPECIES_WIDGET_H
