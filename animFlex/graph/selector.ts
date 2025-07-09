import { BaseSchemes, GetSchemes } from 'rete';
import { AreaExtensions, AreaPlugin } from 'rete-area-plugin';


type ExpectedSchemes = GetSchemes<BaseSchemes['Node'], BaseSchemes['Connection'] & { selected?: boolean }>

export function createSelector<Schemes extends ExpectedSchemes, K>(area: AreaPlugin<Schemes, K>) {
  const selector = AreaExtensions.selector()
  const accumulating = AreaExtensions.accumulateOnCtrl()

  AreaExtensions.selectableNodes(area, selector, { accumulating });

  function unselectConnection(c: Schemes['Connection']) {
    c.selected = false
    area.update('connection', c.id)
  }

  function selectConnection(c: Schemes['Connection']) {
    selector.add({
      id: c.id,
      label: 'connection',
      translate() {},
      unselect() {
        unselectConnection(c)
      },
    }, accumulating.active())
    c.selected = true
    area.update('connection', c.id)
  }

  return {
    selectConnection,
    unselectConnection
  }
}
